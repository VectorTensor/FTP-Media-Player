#include "Anime.grpc.pb.h"

class AnimeServiceImpl final : public anime::AnimeService::Service {

  grpc::Status Ping(grpc::ServerContext *context,
                    const anime::PingRequest *request,
                    anime::PongReply *reply) override;
};
