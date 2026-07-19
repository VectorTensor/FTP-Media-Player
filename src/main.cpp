#include "FTPTextSource.h"
#include <iostream>

#include "lib/config/config.h"
#include "services/anime_service.h"
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>

int ftp_main() {
  std::string host = config::get_env_or("FTP_HOST");
  std::string username = config::get_env_or("FTP_USER");
  std::string password = config::get_env_or("FTP_PASS");
  std::string current_path = "//anime/";

  ftp_handler::FtpConfig config{host, username, password};

  auto client = ftp_handler::create_ftp_client(host, username, password);

  if (!client) {
    std::cerr << "Error creating ftp client" << client.error() << std::endl;
  }

  auto result = ftp_handler::ftp_connect(*client.value());

  auto resources =
      ftp_handler::ftp_list_resources(*client.value(), current_path);

  if (!resources) {
    std::cerr << "Error creating ftp resources" << resources.error()
              << std::endl;
  }

  for (const auto &resource : *resources) {
    std::cout << resource.name << std::endl;
  }
}

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  AnimeServiceImpl service;
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main() {
  RunServer();
  return 0;
}
