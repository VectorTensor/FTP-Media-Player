#pragma once

#include <tl/expected.hpp>
#include <string>
#include <curl/curl.h>

tl::expected<std::string, std::string> test_ftp_text_source();

namespace ftp_handler {


    struct FtpConfig {
        std::string host;
        std::string username;
        std::string password;

    };

    struct ResourceInfo {

        std::string id;
        std::string name;


    };

    struct FtpClient {
        CURL* curl_handler{};
        FtpConfig config;
    };

    tl::expected<FtpClient*, std::string> create_ftp_client(const std::string& host, const std::string& username, const std::string& password);
    tl::expected<void, std::string> ftp_connect(const FtpClient& client);

    tl::expected<std::vector<ResourceInfo>, std::string> ftp_list_resources(const FtpClient& client);

    tl::expected<void, std::string> destroy_curl_handler(const FtpClient* client);




}
