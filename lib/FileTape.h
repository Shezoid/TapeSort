#pragma once

#include "AbstractTape.h"
#include <fstream>
#include <concepts>

#include <chrono>

template<std::integral T>
class FileTape : public AbstractTape<T> {
public:
    FileTape(const std::string &fileName,
             const std::chrono::milliseconds readDataDelay,
             const std::chrono::milliseconds writeDataDelay,
             const std::chrono::milliseconds moveTapeDelay)
        : readDataDelay(readDataDelay),
          writeDataDelay(writeDataDelay),
          moveTapeDelay(moveTapeDelay) {

        file = std::fstream(fileName, std::ios::binary | std::ios::in | std::ios::out);
        if (!file.is_open()) {
            file = std::fstream(fileName, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
            if (!file.is_open()) {
                std::cerr << "Error opening file " << fileName << std::endl;
            }
        }
    }

    T read() override {
        T value;
        file.seekg(current * sizeof(T));
        file.read(
            reinterpret_cast<char *>(&value),
            sizeof(T)
        );
        return value;
    }

    void write(T value) override {
        file.seekp(current * sizeof(T));
        file.write(
            reinterpret_cast<char *>(&value),
            sizeof(T)
        );

        if (!file) {
            std::cerr << "Error writing at index " << current << std::endl;
        }

        if (current >= length) {
            length = current + 1;
        }
    }

    void next() override {
        if (length < current) {
            ++current;
        }
    }

    void previous() override {
        if (current > 0) {
            --current;
        }
    }

    std::size_t getLength() override {
        return length;
    }

    bool hasNext() override {
        return current < length;
    }

    bool hasPrevious() override {
        return current > 0;
    }

    ~FileTape() override {
        file.close();
    }

private:
    std::fstream file;
    std::size_t length = 0;
    std::size_t current = 0;
    std::chrono::milliseconds readDataDelay;
    std::chrono::milliseconds writeDataDelay;
    std::chrono::milliseconds moveTapeDelay;
};
