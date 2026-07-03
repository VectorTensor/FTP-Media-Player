#pragma once

#include <tl/expected.hpp>
#include <string>

tl::expected<std::string, std::string> test_ftp_text_source();