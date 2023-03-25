#include "grpc-server.h"

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <random>

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
    auto res = db->getByLseq(request->lseq(), request->has_limit()
        ? static_cast<int>(request->limit())
        : -1);
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

Status LSeqDatabaseImpl::SyncGet_(ServerContext* context, const SyncGetRequest* request, LSeq* response) {
    auto seq = db->sequenceNumberForReplica(request->replica_id());
    response->set_lseq(dbConnector::generateLseqKey(seq, request->replica_id()));
    return Status::OK;
}

Status LSeqDatabaseImpl::SyncPut_(ServerContext* context, const DBItems* request, ::google::protobuf::Empty* response) {
    std::lock_guard<std::mutex> lockGuard(syncMxs_[request->replica_id()]);
    batchValues batch;
    batch.reserve(request->items_size());
    std::cerr << "Got batch from " << request->replica_id() << std::endl;
    std::cerr << "Items:" << std::endl;
    for (const auto& item : request->items()) {
        batch.emplace_back(item.lseq(), item.key(), item.value());
        std::cerr << "lseq=" << item.lseq() << "; key=" << item.key() << "; value=" << item.value() << std::endl;
    }
    std::cerr << std::endl;
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
    std::cerr << "Sending new batch with size=" << batch.items_size() << std::endl;
    ClientContext context;
    google::protobuf::Empty response;
    auto deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(500);
    context.set_deadline(deadline);

    Status status = client->SyncPut_(&context, batch, &response);
    if (!status.ok()) {
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

    for (auto i : replicaIds) {
        // Connect to remote replica;
        auto channel = grpc::CreateChannel(replicas[i], grpc::InsecureChannelCredentials());

        std::unique_ptr<LSeqDatabase::Stub> client(LSeqDatabase::NewStub(channel));

        for (auto id : syncOrder) {
            auto maxSeq = database->sequenceNumberForReplica(id);
            if (maxSeq < 1) {
                // no data from replica=id
                continue;
            }
            auto remoteLSeq = GetMaxLSeqFromRemoteReplica(client, id);
            std::cerr << "maxSeq=" << maxSeq << " maxRemoteLSeq=" << remoteLSeq << " for id=" << id << std::endl;
            if (remoteLSeq.empty()) {
                // error
                std::cerr << "Cannot get maxLSeq for " << id << " from " << replicas[i] << std::endl;
                continue;
            }
            auto remoteSeq = dbConnector::lseqToSeq(remoteLSeq);
            if (maxSeq <= remoteSeq) {
                // No new data
                continue;
            }

            auto newBatch = DumpBatch(database, remoteLSeq);
            newBatch.set_replica_id(id);
            if (!SendNewBatch(client, newBatch)) {
                std::cerr << "Synchronization data from " << id << " with replica_id=" << replicas[i] << " failed" << std::endl;
            } else {
                std::cerr << "Sync data from " << id << " with " << replicas[i] << " successfully" << std::endl;
            }
        }
    }
}