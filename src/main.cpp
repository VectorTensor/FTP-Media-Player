#include "FTPTextSource.h"
#include <iostream>

int main() {
    auto result = test_ftp_text_source();

    if (result) {
        std::cout << "Result: " << *result << "\n";
    } else {
        std::cerr << "Error: " << result.error() << "\n";
    }

    return 0;
}