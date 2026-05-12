#pragma once

#include "AbstractTape.h"
#include <fstream>
#include <concepts>
#include <iostream>
#include <chrono>
#include <thread>

template<std::integral T>
class FileTape : public AbstractTape<T> {
public:
    FileTape(const std::string &fileName,
             const std::chrono::milliseconds readDataDelay,
             const std::chrono::milliseconds writeDataDelay,
             const std::chrono::milliseconds moveTapeDelay)
        : readDataDelay(readDataDelay),
          writeDataDelay(writeDataDelay),
          moveTapeDelay(moveTapeDelay),
          filePath(fileName) {
        isTempTape = false;
        file = std::fstream(fileName, std::ios::binary | std::ios::in | std::ios::out);
        if (file.is_open()) {
            file.read(reinterpret_cast<char *>(&length), sizeof(std::size_t));
            file.seekg(std::ios::beg);
        } else {
            file = std::fstream(fileName, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
            length = 0;
            if (!file.is_open()) {
                std::cerr << "Error opening file " << fileName << std::endl;
            }
        }
    }

    FileTape(const std::string &fileName,
             const std::chrono::milliseconds readDataDelay,
             const std::chrono::milliseconds writeDataDelay,
             const std::chrono::milliseconds moveTapeDelay,
             bool isTempTape) : FileTape(
        fileName,
        readDataDelay,
        writeDataDelay,
        moveTapeDelay) {
        this->isTempTape = isTempTape;
    }

    void convertFromStream(std::istream& stream) override {
        T value;
        while (stream >> value) {
            write(value);
            next();
        }
        while (hasPrevious()) {
            previous();
        }
    }

    T read() override {
        std::this_thread::sleep_for(readDataDelay);
        T value;
        file.seekg(sizeof(std::size_t) + current * sizeof(T));
        file.read(
            reinterpret_cast<char *>(&value),
            sizeof(T)
        );
        return value;
    }

    void write(T value) override {
        std::this_thread::sleep_for(writeDataDelay);
        file.seekp(sizeof(std::size_t) + current * sizeof(T));
        file.write(
            reinterpret_cast<char *>(&value),
            sizeof(T)
        );

        if (!file) {
            std::cerr << "Error writing at index " << current << std::endl;
        }

        if (current >= length) {
            length = current + 1;
            file.seekp(std::fstream::beg);
            file.write(reinterpret_cast<char *>(&length), sizeof(std::size_t));
        }
    }

    void next() override {
        std::this_thread::sleep_for(moveTapeDelay);
        if (length > current) {
            ++current;
        }
    }

    void previous() override {
        std::this_thread::sleep_for(moveTapeDelay);
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
        if (isTempTape) {
            std::filesystem::remove(filePath);
        }
    }

private:
    bool isTempTape;
    std::string filePath;
    std::fstream file;
    std::size_t length = 0;
    std::size_t current = 0;
    std::chrono::milliseconds readDataDelay;
    std::chrono::milliseconds writeDataDelay;
    std::chrono::milliseconds moveTapeDelay;
};
