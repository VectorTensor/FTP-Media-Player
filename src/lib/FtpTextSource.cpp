#include "FTPTextSource.h"
#include <curl/curl.h>
#include <iostream>

tl::expected<std::string, std::string> test_ftp_text_source() {
    std::cout << "FtpTextSource stub running. curl version: "
              << curl_version() << "\n";

    if (curl_version() == nullptr) {
        return tl::unexpected("Failed to get curl version");
    }

    return std::string("stub success");
}