#include "grpc-server.h"

#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <random>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

LSeqDatabaseImpl::LSeqDatabaseImpl(const YAMLConfig& config, dbConnector* database) : db(database), cfg(config) {
    syncMxs_.resize(config.getMaxReplicaId());
}

Status LSeqDatabaseImpl::GetValue(ServerContext* context, const ReplicaKey* request, Value* response) {
    pureReplyValue res;
    if (!request->has_replica_id()) {
        res = db->get(request->key());
    } else {
        res = db->get(request->key(), request->replica_id());
    }
    if (!std::get<1>(res).ok()) {
        return {grpc::StatusCode::UNAVAILABLE, std::get<1>(res).ToString()};
    }
    response->set_lseq(std::get<0>(res));
    response->set_value(std::get<2>(res));
    return Status::OK;
}

Status LSeqDatabaseImpl::Put(ServerContext* context, const PutRequest* request, LSeq* response) {
    auto res = db->put(request->key(), request->value());
    if (!std::get<1>(res).ok()) {
        return {grpc::StatusCode::ABORTED, res.second.ToString()};
    }
    response->set_lseq(res.first);
    return Status::OK;
}

Status LSeqDatabaseImpl::SeekGet(ServerContext* context, const SeekGetRequest* request, DBItems* response) {
    replyBatchFormat res;
    int limit = request->has_limit() ? static_cast<int>(request->limit()) : -1;
    const auto& lseq = request->lseq();
    if (request->has_key()) {
        res = db->getValuesForKey(request->key(), dbConnector::lseqToSeq(lseq), std::stoi(dbConnector::lseqToReplicaId(lseq)), limit);
    } else {
        res = db->getByLseq(lseq, limit);
    }

    if (!res.first.ok()) {
        return {grpc::StatusCode::UNAVAILABLE, res.first.ToString()};
    }
    for (auto item : res.second) {
        auto proto_item = response->add_items();
        proto_item->set_lseq(std::get<0>(item));
        proto_item->set_key(dbConnector::stampedKeyToRealKey(std::get<1>(item)));
        proto_item->set_value(std::get<2>(item));
    }
    return Status::OK;
}

Status LSeqDatabaseImpl::GetReplicaEvents(ServerContext* context, const EventsRequest* request, DBItems* response) {
    SeekGetRequest req;
    if (request->has_limit()) {
        req.set_limit(request->limit());
    }
    if (request->has_key()) {
        req.set_key(request->key());
    }
    if (request->has_lseq()) {
        req.set_lseq(request->lseq());
    } else {
        req.set_lseq(dbConnector::generateLseqKey(0, request->replica_id()));
    }
    return SeekGet(context, &req, response);
}

Status LSeqDatabaseImpl::SyncGet_(ServerContext* context, const SyncGetRequest* request, LSeq* response) {
    auto seq = db->sequenceNumberForReplica(request->replica_id());
    response->set_lseq(dbConnector::generateLseqKey(seq, request->replica_id()));
    return Status::OK;
}

Status LSeqDatabaseImpl::SyncPut_(ServerContext* context, const DBItems* request, ::google::protobuf::Empty* response) {
    std::lock_guard<std::mutex> lockGuard(syncMxs_[request->replica_id()]);
    batchValues batch;
    batch.reserve(request->items_size());
    for (const auto& item : request->items()) {
        batch.emplace_back(item.lseq(), item.key(), item.value());
    }
    auto res = db->putBatch(batch);
    if (!res.ok()) {
        return {grpc::StatusCode::ABORTED, res.ToString()};
    }
    return Status::OK;
}

void RunServer(const YAMLConfig& config, dbConnector* database) {
    std::string server_address = "0.0.0.0:" + std::to_string(config.getGRPCConfig().port);
    LSeqDatabaseImpl service(config, database);

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

std::string GetMaxLSeqFromRemoteReplica(const std::unique_ptr<LSeqDatabase::Stub>& client, size_t replicaId) {
    ClientContext context;
    SyncGetRequest request;
    LSeq response;

    request.set_replica_id(static_cast<int32_t>(replicaId));

    Status status = client->SyncGet_(&context, request, &response);
    if (!status.ok()) {
        std::cerr << status.error_message() << std::endl;
        return "";
    }
    return response.lseq();
}

DBItems DumpBatch(dbConnector* database, const std::string& lseq) {
    auto res = database->getByLseq(lseq);
    if (!res.first.ok()) {
        return {};
    }
    DBItems batch;
    for (auto item : res.second) {
        auto proto_item = batch.add_items();
        proto_item->set_lseq(std::get<0>(item));
        proto_item->set_key(std::get<1>(item));
        proto_item->set_value(std::get<2>(item));
    }
    return batch;
}

bool SendNewBatch(const std::unique_ptr<LSeqDatabase::Stub>& client, const DBItems& batch) {
    ClientContext context;
    google::protobuf::Empty response;
    auto deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(500);
    context.set_deadline(deadline);

    Status status = client->SyncPut_(&context, batch, &response);
    if (!status.ok()) {
        std::cerr << status.error_message() << std::endl;
        return false;
    }
    return true;
}

void SyncLoop(const YAMLConfig& config, dbConnector* database) {
    const auto& replicas = config.getReplicas();
    std::vector<size_t> replicaIds(replicas.size());
    std::vector<size_t> syncOrder(config.getMaxReplicaId());
    std::iota(replicaIds.begin(), replicaIds.end(), static_cast<size_t>(0));
    std::iota(syncOrder.begin(), syncOrder.end(), static_cast<size_t>(0));

    // Shuffle replicas for sync to avoid a lot of mutex blocking
    std::random_device rd;
    std::mt19937 rnd(rd());

    std::shuffle(replicaIds.begin(), replicaIds.end(), rnd);
    std::shuffle(syncOrder.begin(), syncOrder.end(), rnd);

    // iterate over neighboring replicas in random order
    for (auto i : replicaIds) {
        // Connect to remote replica;
        auto channel = grpc::CreateChannel(replicas[i], grpc::InsecureChannelCredentials());

        std::unique_ptr<LSeqDatabase::Stub> client(LSeqDatabase::NewStub(channel));

        // iterate over known replicas in random order
        for (auto id : syncOrder) {
            auto maxSeq = database->sequenceNumberForReplica(id);
            if (maxSeq < 1) {
                // no data from replica=id
                continue;
            }
            auto remoteLSeq = GetMaxLSeqFromRemoteReplica(client, id);
            if (remoteLSeq.empty()) {
                // error
                std::cerr << "Failed to get maxLSeq(" << id << ") from " << replicas[i] << std::endl;
                continue;
            }
            auto remoteSeq = dbConnector::lseqToSeq(remoteLSeq);
            if (maxSeq <= remoteSeq) {
                // No new data
                continue;
            } else {
                std::cout << "Trying to sync data from " << id << " with " << replicas[i] << "\n";
                std::cout << "Current localMaxSeq(" << id << ")=" << maxSeq << "; "
                          << "remoteMaxSeq(" << replicas[i] << ", " << id << ")=" << remoteSeq << std::endl;
            }

            auto newBatch = DumpBatch(database, remoteLSeq);
            if (newBatch.items_size()) {
                newBatch.set_replica_id(id);
                if (!SendNewBatch(client, newBatch)) {
                    std::cerr << "Failed to send batch to " << replicas[i] << std::endl;
                } else {
                    std::cout << "Data has been successfully synchronized" << std::endl;
                }
                std::this_thread::sleep_for(100ms);
            }
        }
    }
}