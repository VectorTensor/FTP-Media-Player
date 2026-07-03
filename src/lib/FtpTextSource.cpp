#include "FtpTextSource.h"
#include <curl/curl.h>
#include <iostream>

void test_ftp_text_source() {
    std::cout << "FtpTextSource stub running. curl version: "
              << curl_version() << "\n";
}