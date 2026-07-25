#include "anime_service.h"
#include "Anime.pb.h"
#include "FTPTextSource.h"
#include "grpcpp/server_context.h"
#include "lib/ftp_helper/ftp_helper.h"
#include <format>
#include <grpcpp/support/status.h>
#include <vector>

grpc::Status AnimeServiceImpl::Ping(grpc::ServerContext *context,
                                    const anime::PingRequest *request,
                                    anime::PongReply *reply) {
  std::cout << "Received: " << request->message() << std::endl;
  reply->set_message("Pong: " + request->message());
  return grpc::Status::OK;
}

grpc::Status AnimeServiceImpl::ListAnime(grpc::ServerContext *context,
                                         const anime::ListAnimeRequest *request,
                                         anime::ListAnimeResponse *reply) {

  std::cout << "Calling ListAnime " << std::endl;

  ftp_handler::FtpClient c = get_ftp_client();

  auto resources = ftp_handler::ftp_list_resources(c, "//anime/");

  if (!resources) {
    std::cerr << "Error creating ftp resources" << resources.error()
              << std::endl;
  }

  std::vector<std::string> *animes = new std::vector<std::string>();

  for (const auto &resource : *resources) {
    anime::AnimeData *data = reply->add_animes();
    data->set_name(resource.name);
    data->set_url(std::format("{}//anime/{}", c.config.host, resource.name));
  }

  return grpc::Status::OK;
}
