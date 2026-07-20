#include "Anime.grpc.pb.h"
#include "Anime.pb.h"
#include "grpcpp/server_context.h"
#include <grpcpp/support/status.h>

class AnimeServiceImpl final : public anime::AnimeService::Service {

  grpc::Status Ping(grpc::ServerContext *context,
                    const anime::PingRequest *request,
                    anime::PongReply *reply) override;

  grpc::Status ListAnime(grpc::ServerContext *context,
                         const anime::ListAnimeRequest *request,
                         anime::ListAnimeResponse *reply) override;
};
