#include "FTPTextSource.h"
#include <iostream>

#include "lib/config/config.h"

int main() {
    std::string host     = config::get_env_or("FTP_HOST");
    std::string username = config::get_env_or("FTP_USER");
    std::string password = config::get_env_or("FTP_PASS");

    ftp_handler::FtpConfig config{
        host,
        username,
        password
    };

    auto client = ftp_handler::create_ftp_client(
        host,
        username,
        password
        );

    if (!client) {
        std::cerr << "Error creating ftp client" <<client.error() <<std::endl;
    }

    auto result = ftp_handler::ftp_connect(
        *client.value()
        );

    auto resources = ftp_handler::ftp_list_resources(
        *client.value()
        );

    if (!resources) {
        std::cerr << "Error creating ftp resources" << resources.error()<<std::endl;
    }

    for (const auto& resource : *resources) {
        std::cout << resource.name << std::endl;
    }





}
