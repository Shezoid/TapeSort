#include <fstream>
#include <string>
#include <unordered_map>

class EnvParser {
public:
    std::unordered_map<std::string, std::string >& parse(std::string configPath) {
        std::ifstream file(configPath);
        if (!file.is_open()) {
            throw std::invalid_argument("Invalid configuration file path");
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line[0] == '#') continue;

            std::size_t position = line.find('=');
            if (position == std::string::npos) continue;

            std::string key = line.substr(0, position);
            std::string value = line.substr(position + 1);
            configs[key] = value;
        }

        return configs;
    }

private:
    std::unordered_map<std::string, std::string> configs;
};
