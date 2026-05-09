
#include <chrono>
#include <cstddef>
#include <string>

struct Config {
        std::size_t length;
        std::string InputFileName;
        std::string OutputFileName;
        std::chrono::milliseconds readDataDelay;
        std::chrono::milliseconds writeDataDelay;
        std::chrono::milliseconds moveTapeDelay;
};