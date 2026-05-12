#pragma once

#include <chrono>
#include <cstddef>
#include <filesystem>
#include <string>

const std::size_t MAX_MEMORY_CAPACITY = 4 * 1024;

struct Config {
    Config() = delete;

    Config(
        std::size_t maxMemoryCapacity,
        std::string tempDirectoryPath,
        std::chrono::milliseconds readDataDelay,
        std::chrono::milliseconds writeDataDelay,
        std::chrono::milliseconds moveTapeDelay) : maxMemoryCapacity(maxMemoryCapacity),
                                                   tempDirectoryPath(tempDirectoryPath),
                                                   readDataDelay(readDataDelay),
                                                   writeDataDelay(writeDataDelay),
                                                   moveTapeDelay(moveTapeDelay) {
    }

    size_t maxMemoryCapacity;
    std::string tempDirectoryPath;
    std::chrono::milliseconds readDataDelay;
    std::chrono::milliseconds writeDataDelay;
    std::chrono::milliseconds moveTapeDelay;

    class Builder {
    public:
        Builder() = default;

        Builder &withMaxMemoryCapacity(size_t maxMemoryCapacity) {
            this->maxMemoryCapacity = maxMemoryCapacity;
            return *this;
        }

        Builder &withTempDirectoryPath(const std::string &tempDirectoryPath) {
            this->tempDirectoryPath = tempDirectoryPath;
            return *this;
        }

        Builder &withReadDataDelay(std::chrono::milliseconds readDataDelay) {
            this->readDataDelay = readDataDelay;
            return *this;
        }

        Builder &withWriteDataDelay(std::chrono::milliseconds writeDataDelay) {
            this->writeDataDelay = writeDataDelay;
            return *this;
        }

        Builder &withMoveDataDelay(std::chrono::milliseconds moveDataDelay) {
            this->moveTapeDelay = moveDataDelay;
            return *this;
        }

        Config build() {
            return {
                maxMemoryCapacity,
                tempDirectoryPath,
                readDataDelay,
                writeDataDelay,
                moveTapeDelay};
        }

    private:
        size_t maxMemoryCapacity = MAX_MEMORY_CAPACITY;
        std::string tempDirectoryPath = std::filesystem::path(PROJECT_ROOT) / "tmp";
        std::chrono::milliseconds readDataDelay = std::chrono::milliseconds(0);
        std::chrono::milliseconds writeDataDelay = std::chrono::milliseconds(0);
        std::chrono::milliseconds moveTapeDelay = std::chrono::milliseconds(0);
    };
};

