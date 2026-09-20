#include <iostream>
#include "config.h"
#include <cstdlib>
#include <fstream>
#include <string>
#include <unordered_map>

namespace config{
    std::unordered_map<std::string, std::string> load_env_file(
        const std::string& path
    ) {
        std::unordered_map<std::string, std::string> values;

        std::ifstream file(path);
        if (!file) {
            return values;
        }

        std::string line;

        while (std::getline(file, line)) {
            // Skip empty lines and comments
            if (line.empty() || line[0] == '#')
                continue;

            auto pos = line.find('=');
            if (pos == std::string::npos)
                continue;

            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            values[key] = value;
        }

        return values;
    }
    std::string get_env_or(const char* name, const std::string& fallback ) {

        // Environment variable takes precedence
        if (const char* value = std::getenv(name)) {
            return std::string(value);
        }

        static const auto config =
            load_env_file("/vault/secrets/config");

        auto it = config.find(name);

        return it != config.end()
            ? it->second
            : fallback;



    }
}
