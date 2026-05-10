
#include <chrono>
#include <cstddef>
#include <string>

struct Config {
        size_t maxMemoryCapacity;
        size_t length;
        std::string inputFileName;
        std::string outputFileName;
        std::string tempDirectoryPath;
        std::chrono::milliseconds readDataDelay;
        std::chrono::milliseconds writeDataDelay;
        std::chrono::milliseconds moveTapeDelay;
};