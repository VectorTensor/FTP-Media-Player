#pragma once

#include <iostream>
namespace config {
    std::string get_env_or(const char* name, const std::string& fallback = "");
}

