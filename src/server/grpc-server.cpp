#include "grpc-server.h"

#include <stdexcept>

LSeqDatabaseImpl::LSeqDatabaseImpl(const YAMLConfig& config) : db(config) {
}

Status LSeqDatabaseImpl::GetValue(ServerContext* context, const ReplicaKey* request, Value* response) {
    pureReplyValue res;
    if (!request->has_replica_id()) {
        res = db.get(request->key());
    } else {
        res = db.get(request->key(), request->replica_id());
    }
    if (!std::get<1>(res).ok()) {
        return {grpc::StatusCode::UNAVAILABLE, std::get<1>(res).ToString()};
    }
    response->set_value(std::get<2>(res));
    return Status::OK;
}

Status LSeqDatabaseImpl::Put(ServerContext* context, const PutRequest* request, LSeq* response) {
    auto res = db.put(request->key(), request->value());
    if (!std::get<1>(res).ok()) {
        return {grpc::StatusCode::ABORTED, res.second.ToString()};
    }
    response->set_lseq(res.first);
    return Status::OK;
}

Status LSeqDatabaseImpl::SeekGet(ServerContext* context, const SeekGetRequest* request, DBItems* response) {
    auto res = db.getByLseq(request->lseq(), request->has_limit()
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
    throw std::logic_error("Not implemented");
}

Status LSeqDatabaseImpl::SyncPut_(ServerContext* context, const DBItems* request, ::google::protobuf::Empty* response) {
    throw std::logic_error("Not implemented");
}

void RunServer(const YAMLConfig& config) {
    std::string server_address = "0.0.0.0:" + std::to_string(config.getGRPCConfig().port);
    LSeqDatabaseImpl service(config);

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}