#include <iostream>
#include "config.h"

namespace config{

    std::string get_env_or(const char* name, const std::string& fallback ) {

        const char* value = std::getenv(name);
        return value ? std::string(value) : fallback;



    }
}