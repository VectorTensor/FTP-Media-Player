#include "FTPTextSource.h"
#include <curl/curl.h>
#include <sstream>
#include <iostream>

tl::expected<std::string, std::string> test_ftp_text_source() {
    std::cout << "FtpTextSource stub running. curl version: "
              << curl_version() << "\n";

    if (curl_version() == nullptr) {
        return tl::unexpected("Failed to get curl version");
    }

    return std::string("stub success");
}

namespace ftp_handler {


    tl::expected<FtpClient*, std::string> create_ftp_client(const std::string& host, const std::string& username, const std::string& password) {

        CURL* curl = curl_easy_init();
        if (!curl) {

            return tl::unexpected("Failed to initialize curl handle");

        }

        const auto config = FtpConfig(
            host,
            username,
            password
            );


        auto client = new FtpClient(curl , config);

        return client;

    }

    tl::expected<void, std::string> destroy_curl_handler(const FtpClient* client) {

        curl_easy_cleanup(client->curl_handler);
        delete client;
        return{};

    }

    void apply_common_opts(const FtpClient& client) {
        curl_easy_setopt(client.curl_handler, CURLOPT_USERNAME, client.config.username.c_str());
        curl_easy_setopt(client.curl_handler, CURLOPT_PASSWORD, client.config.password.c_str());
        curl_easy_setopt(client.curl_handler, CURLOPT_CONNECTTIMEOUT, 10L);
        curl_easy_setopt(client.curl_handler, CURLOPT_URL, client.config.host.c_str());
    }

    tl::expected<void, std::string> ftp_connect(const FtpClient& client) {

        curl_easy_setopt(client.curl_handler, CURLOPT_URL, client.config.host.c_str());
        curl_easy_setopt(client.curl_handler, CURLOPT_USERNAME, client.config.username.c_str());
        curl_easy_setopt(client.curl_handler, CURLOPT_PASSWORD, client.config.password.c_str());
        curl_easy_setopt(client.curl_handler, CURLOPT_CONNECT_ONLY, 0L);
        curl_easy_setopt(client.curl_handler, CURLOPT_CONNECTTIMEOUT, 10l);

        CURLcode res = curl_easy_perform(client.curl_handler);


        if (res != CURLE_OK) {
            return tl::unexpected("curl_easy_perform() failed");
        }
        return {};



    }
    size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
        auto* out = static_cast<std::string*>(userdata);
        out->append(ptr, size * nmemb);
        return size * nmemb;
    }

    tl::expected<std::vector<ResourceInfo>, std::string> ftp_list_resources(const FtpClient& client) {

        std::string listing;
        apply_common_opts(client);
        curl_easy_setopt(client.curl_handler, CURLOPT_DIRLISTONLY, 1L);
        curl_easy_setopt(client.curl_handler, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(client.curl_handler, CURLOPT_WRITEDATA, &listing);

        CURLcode res = curl_easy_perform(client.curl_handler);
        if (res != CURLE_OK) {
            return tl::unexpected(std::string("FTP list failed: ") + curl_easy_strerror(res));
        }

        std::vector<ResourceInfo> resources;

        std::istringstream stream(listing);

        std::string filename;
        int count = 1;
        while (std::getline(stream, filename)) {
            if (!filename.empty() &&  filename.back() == '\r') {

                filename.pop_back();

            }

            resources.push_back(ResourceInfo{std::to_string(count), filename});

            count++;

        }

        return resources;


    }
}

