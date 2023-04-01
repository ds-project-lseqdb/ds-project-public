// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: lseqDb.proto

#include "lseqDb.pb.h"
#include "lseqDb.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace lseqdb {

static const char* LSeqDatabase_method_names[] = {
  "/lseqdb.LSeqDatabase/GetValue",
  "/lseqdb.LSeqDatabase/Put",
  "/lseqdb.LSeqDatabase/SeekGet",
  "/lseqdb.LSeqDatabase/GetReplicaEvents",
  "/lseqdb.LSeqDatabase/GetConfig",
  "/lseqdb.LSeqDatabase/SyncGet_",
  "/lseqdb.LSeqDatabase/SyncPut_",
};

std::unique_ptr< LSeqDatabase::Stub> LSeqDatabase::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< LSeqDatabase::Stub> stub(new LSeqDatabase::Stub(channel, options));
  return stub;
}

LSeqDatabase::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_GetValue_(LSeqDatabase_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Put_(LSeqDatabase_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_SeekGet_(LSeqDatabase_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetReplicaEvents_(LSeqDatabase_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetConfig_(LSeqDatabase_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_SyncGet__(LSeqDatabase_method_names[5], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_SyncPut__(LSeqDatabase_method_names[6], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status LSeqDatabase::Stub::GetValue(::grpc::ClientContext* context, const ::lseqdb::ReplicaKey& request, ::lseqdb::Value* response) {
  return ::grpc::internal::BlockingUnaryCall< ::lseqdb::ReplicaKey, ::lseqdb::Value, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetValue_, context, request, response);
}

void LSeqDatabase::Stub::async::GetValue(::grpc::ClientContext* context, const ::lseqdb::ReplicaKey* request, ::lseqdb::Value* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::lseqdb::ReplicaKey, ::lseqdb::Value, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetValue_, context, request, response, std::move(f));
}

void LSeqDatabase::Stub::async::GetValue(::grpc::ClientContext* context, const ::lseqdb::ReplicaKey* request, ::lseqdb::Value* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetValue_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::lseqdb::Value>* LSeqDatabase::Stub::PrepareAsyncGetValueRaw(::grpc::ClientContext* context, const ::lseqdb::ReplicaKey& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::lseqdb::Value, ::lseqdb::ReplicaKey, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetValue_, context, request);
}

::grpc::ClientAsyncResponseReader< ::lseqdb::Value>* LSeqDatabase::Stub::AsyncGetValueRaw(::grpc::ClientContext* context, const ::lseqdb::ReplicaKey& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetValueRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status LSeqDatabase::Stub::Put(::grpc::ClientContext* context, const ::lseqdb::PutRequest& request, ::lseqdb::LSeq* response) {
  return ::grpc::internal::BlockingUnaryCall< ::lseqdb::PutRequest, ::lseqdb::LSeq, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Put_, context, request, response);
}

void LSeqDatabase::Stub::async::Put(::grpc::ClientContext* context, const ::lseqdb::PutRequest* request, ::lseqdb::LSeq* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::lseqdb::PutRequest, ::lseqdb::LSeq, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Put_, context, request, response, std::move(f));
}

void LSeqDatabase::Stub::async::Put(::grpc::ClientContext* context, const ::lseqdb::PutRequest* request, ::lseqdb::LSeq* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Put_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::lseqdb::LSeq>* LSeqDatabase::Stub::PrepareAsyncPutRaw(::grpc::ClientContext* context, const ::lseqdb::PutRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::lseqdb::LSeq, ::lseqdb::PutRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Put_, context, request);
}

::grpc::ClientAsyncResponseReader< ::lseqdb::LSeq>* LSeqDatabase::Stub::AsyncPutRaw(::grpc::ClientContext* context, const ::lseqdb::PutRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncPutRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status LSeqDatabase::Stub::SeekGet(::grpc::ClientContext* context, const ::lseqdb::SeekGetRequest& request, ::lseqdb::DBItems* response) {
  return ::grpc::internal::BlockingUnaryCall< ::lseqdb::SeekGetRequest, ::lseqdb::DBItems, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_SeekGet_, context, request, response);
}

void LSeqDatabase::Stub::async::SeekGet(::grpc::ClientContext* context, const ::lseqdb::SeekGetRequest* request, ::lseqdb::DBItems* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::lseqdb::SeekGetRequest, ::lseqdb::DBItems, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SeekGet_, context, request, response, std::move(f));
}

void LSeqDatabase::Stub::async::SeekGet(::grpc::ClientContext* context, const ::lseqdb::SeekGetRequest* request, ::lseqdb::DBItems* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SeekGet_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::lseqdb::DBItems>* LSeqDatabase::Stub::PrepareAsyncSeekGetRaw(::grpc::ClientContext* context, const ::lseqdb::SeekGetRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::lseqdb::DBItems, ::lseqdb::SeekGetRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_SeekGet_, context, request);
}

::grpc::ClientAsyncResponseReader< ::lseqdb::DBItems>* LSeqDatabase::Stub::AsyncSeekGetRaw(::grpc::ClientContext* context, const ::lseqdb::SeekGetRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncSeekGetRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status LSeqDatabase::Stub::GetReplicaEvents(::grpc::ClientContext* context, const ::lseqdb::EventsRequest& request, ::lseqdb::DBItems* response) {
  return ::grpc::internal::BlockingUnaryCall< ::lseqdb::EventsRequest, ::lseqdb::DBItems, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetReplicaEvents_, context, request, response);
}

void LSeqDatabase::Stub::async::GetReplicaEvents(::grpc::ClientContext* context, const ::lseqdb::EventsRequest* request, ::lseqdb::DBItems* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::lseqdb::EventsRequest, ::lseqdb::DBItems, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetReplicaEvents_, context, request, response, std::move(f));
}

void LSeqDatabase::Stub::async::GetReplicaEvents(::grpc::ClientContext* context, const ::lseqdb::EventsRequest* request, ::lseqdb::DBItems* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetReplicaEvents_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::lseqdb::DBItems>* LSeqDatabase::Stub::PrepareAsyncGetReplicaEventsRaw(::grpc::ClientContext* context, const ::lseqdb::EventsRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::lseqdb::DBItems, ::lseqdb::EventsRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetReplicaEvents_, context, request);
}

::grpc::ClientAsyncResponseReader< ::lseqdb::DBItems>* LSeqDatabase::Stub::AsyncGetReplicaEventsRaw(::grpc::ClientContext* context, const ::lseqdb::EventsRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetReplicaEventsRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status LSeqDatabase::Stub::GetConfig(::grpc::ClientContext* context, const ::google::protobuf::Empty& request, ::lseqdb::Config* response) {
  return ::grpc::internal::BlockingUnaryCall< ::google::protobuf::Empty, ::lseqdb::Config, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetConfig_, context, request, response);
}

void LSeqDatabase::Stub::async::GetConfig(::grpc::ClientContext* context, const ::google::protobuf::Empty* request, ::lseqdb::Config* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::google::protobuf::Empty, ::lseqdb::Config, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetConfig_, context, request, response, std::move(f));
}

void LSeqDatabase::Stub::async::GetConfig(::grpc::ClientContext* context, const ::google::protobuf::Empty* request, ::lseqdb::Config* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetConfig_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::lseqdb::Config>* LSeqDatabase::Stub::PrepareAsyncGetConfigRaw(::grpc::ClientContext* context, const ::google::protobuf::Empty& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::lseqdb::Config, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetConfig_, context, request);
}

::grpc::ClientAsyncResponseReader< ::lseqdb::Config>* LSeqDatabase::Stub::AsyncGetConfigRaw(::grpc::ClientContext* context, const ::google::protobuf::Empty& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetConfigRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status LSeqDatabase::Stub::SyncGet_(::grpc::ClientContext* context, const ::lseqdb::SyncGetRequest& request, ::lseqdb::LSeq* response) {
  return ::grpc::internal::BlockingUnaryCall< ::lseqdb::SyncGetRequest, ::lseqdb::LSeq, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_SyncGet__, context, request, response);
}

void LSeqDatabase::Stub::async::SyncGet_(::grpc::ClientContext* context, const ::lseqdb::SyncGetRequest* request, ::lseqdb::LSeq* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::lseqdb::SyncGetRequest, ::lseqdb::LSeq, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SyncGet__, context, request, response, std::move(f));
}

void LSeqDatabase::Stub::async::SyncGet_(::grpc::ClientContext* context, const ::lseqdb::SyncGetRequest* request, ::lseqdb::LSeq* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SyncGet__, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::lseqdb::LSeq>* LSeqDatabase::Stub::PrepareAsyncSyncGet_Raw(::grpc::ClientContext* context, const ::lseqdb::SyncGetRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::lseqdb::LSeq, ::lseqdb::SyncGetRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_SyncGet__, context, request);
}

::grpc::ClientAsyncResponseReader< ::lseqdb::LSeq>* LSeqDatabase::Stub::AsyncSyncGet_Raw(::grpc::ClientContext* context, const ::lseqdb::SyncGetRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncSyncGet_Raw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status LSeqDatabase::Stub::SyncPut_(::grpc::ClientContext* context, const ::lseqdb::DBItems& request, ::google::protobuf::Empty* response) {
  return ::grpc::internal::BlockingUnaryCall< ::lseqdb::DBItems, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_SyncPut__, context, request, response);
}

void LSeqDatabase::Stub::async::SyncPut_(::grpc::ClientContext* context, const ::lseqdb::DBItems* request, ::google::protobuf::Empty* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::lseqdb::DBItems, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SyncPut__, context, request, response, std::move(f));
}

void LSeqDatabase::Stub::async::SyncPut_(::grpc::ClientContext* context, const ::lseqdb::DBItems* request, ::google::protobuf::Empty* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SyncPut__, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::google::protobuf::Empty>* LSeqDatabase::Stub::PrepareAsyncSyncPut_Raw(::grpc::ClientContext* context, const ::lseqdb::DBItems& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::google::protobuf::Empty, ::lseqdb::DBItems, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_SyncPut__, context, request);
}

::grpc::ClientAsyncResponseReader< ::google::protobuf::Empty>* LSeqDatabase::Stub::AsyncSyncPut_Raw(::grpc::ClientContext* context, const ::lseqdb::DBItems& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncSyncPut_Raw(context, request, cq);
  result->StartCall();
  return result;
}

LSeqDatabase::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      LSeqDatabase_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< LSeqDatabase::Service, ::lseqdb::ReplicaKey, ::lseqdb::Value, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](LSeqDatabase::Service* service,
             ::grpc::ServerContext* ctx,
             const ::lseqdb::ReplicaKey* req,
             ::lseqdb::Value* resp) {
               return service->GetValue(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      LSeqDatabase_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< LSeqDatabase::Service, ::lseqdb::PutRequest, ::lseqdb::LSeq, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](LSeqDatabase::Service* service,
             ::grpc::ServerContext* ctx,
             const ::lseqdb::PutRequest* req,
             ::lseqdb::LSeq* resp) {
               return service->Put(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      LSeqDatabase_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< LSeqDatabase::Service, ::lseqdb::SeekGetRequest, ::lseqdb::DBItems, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](LSeqDatabase::Service* service,
             ::grpc::ServerContext* ctx,
             const ::lseqdb::SeekGetRequest* req,
             ::lseqdb::DBItems* resp) {
               return service->SeekGet(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      LSeqDatabase_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< LSeqDatabase::Service, ::lseqdb::EventsRequest, ::lseqdb::DBItems, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](LSeqDatabase::Service* service,
             ::grpc::ServerContext* ctx,
             const ::lseqdb::EventsRequest* req,
             ::lseqdb::DBItems* resp) {
               return service->GetReplicaEvents(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      LSeqDatabase_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< LSeqDatabase::Service, ::google::protobuf::Empty, ::lseqdb::Config, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](LSeqDatabase::Service* service,
             ::grpc::ServerContext* ctx,
             const ::google::protobuf::Empty* req,
             ::lseqdb::Config* resp) {
               return service->GetConfig(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      LSeqDatabase_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< LSeqDatabase::Service, ::lseqdb::SyncGetRequest, ::lseqdb::LSeq, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](LSeqDatabase::Service* service,
             ::grpc::ServerContext* ctx,
             const ::lseqdb::SyncGetRequest* req,
             ::lseqdb::LSeq* resp) {
               return service->SyncGet_(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      LSeqDatabase_method_names[6],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< LSeqDatabase::Service, ::lseqdb::DBItems, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](LSeqDatabase::Service* service,
             ::grpc::ServerContext* ctx,
             const ::lseqdb::DBItems* req,
             ::google::protobuf::Empty* resp) {
               return service->SyncPut_(ctx, req, resp);
             }, this)));
}

LSeqDatabase::Service::~Service() {
}

::grpc::Status LSeqDatabase::Service::GetValue(::grpc::ServerContext* context, const ::lseqdb::ReplicaKey* request, ::lseqdb::Value* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status LSeqDatabase::Service::Put(::grpc::ServerContext* context, const ::lseqdb::PutRequest* request, ::lseqdb::LSeq* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status LSeqDatabase::Service::SeekGet(::grpc::ServerContext* context, const ::lseqdb::SeekGetRequest* request, ::lseqdb::DBItems* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status LSeqDatabase::Service::GetReplicaEvents(::grpc::ServerContext* context, const ::lseqdb::EventsRequest* request, ::lseqdb::DBItems* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status LSeqDatabase::Service::GetConfig(::grpc::ServerContext* context, const ::google::protobuf::Empty* request, ::lseqdb::Config* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status LSeqDatabase::Service::SyncGet_(::grpc::ServerContext* context, const ::lseqdb::SyncGetRequest* request, ::lseqdb::LSeq* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status LSeqDatabase::Service::SyncPut_(::grpc::ServerContext* context, const ::lseqdb::DBItems* request, ::google::protobuf::Empty* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace lseqdb

