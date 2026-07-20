
#include "ftp_helper.h"
#include "lib/config/config.h"
#include <curl/curl.h>
#include <string>

ftp_handler::FtpClient &get_ftp_client() {

  std::string host = config::get_env_or("FTP_HOST");
  std::string username = config::get_env_or("FTP_USER");
  std::string password = config::get_env_or("FTP_PASS");
  std::string current_path = "//anime/";

  ftp_handler::FtpConfig config{host, username, password};

  auto client = ftp_handler::create_ftp_client(host, username, password);

  if (!client) {
    std::cerr << "Error creating ftp client" << client.error() << std::endl;
  }

  return *client.value();
}
