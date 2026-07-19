#include "anime_service.h"

grpc::Status AnimeServiceImpl::Ping(grpc::ServerContext *context,
                                    const anime::PingRequest *request,
                                    anime::PongReply *reply) {
  std::cout << "Received: " << request->message() << std::endl;
  reply->set_message("Pong: " + request->message());
  return grpc::Status::OK;
}
