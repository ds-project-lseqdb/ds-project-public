#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <memory>
#include <deque>
#include <mutex>

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "src/proto-src/lseqDb.grpc.pb.h"
#include "src/proto-src/lseqDb.pb.h"

#include "src/db/dbConnector.hpp"

using lseqdb::LSeqDatabase;

using grpc::ServerContext;
using grpc::Status;

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReaderWriter;

using lseqdb::DBItems;
using lseqdb::Key;
using lseqdb::LSeq;
using lseqdb::PutRequest;
using lseqdb::ReplicaKey;
using lseqdb::SeekGetRequest;
using lseqdb::SyncGetRequest;
using lseqdb::Value;

class LSeqDatabaseImpl final : public LSeqDatabase::Service {
public:
    LSeqDatabaseImpl(const YAMLConfig& config, dbConnector* database);
public:
    Status GetValue(ServerContext* context, const ReplicaKey* request, Value* response) override;
    Status Put(ServerContext* context, const PutRequest* request, LSeq* response) override;
    Status SeekGet(ServerContext* context, const SeekGetRequest* request, DBItems* response) override;
    Status Delete(ServerContext* context, const Key* request, google::protobuf::Empty* response) override {
        throw std::logic_error("Not implemented. Use tombstones yourself.");
    }

public:
    Status SyncGet_(ServerContext* context, const SyncGetRequest* request, LSeq* response) override;
    Status SyncPut_(ServerContext* context, const DBItems* request, ::google::protobuf::Empty* response) override;
private:
    std::deque<std::mutex> syncMxs_;

private:
    dbConnector* db;
    const YAMLConfig& cfg;
};

void RunServer(const YAMLConfig& config, dbConnector* database);
void SyncLoop(const YAMLConfig& config, dbConnector* database);
