#pragma once
#include <concepts>

#include "Config.h"
#include "FileTape.h"
#include "AbstractTapeFactory.h"


template<std::integral T>
class FileTapeFactory : public AbstractTapeFactory<T> {
public:
    FileTapeFactory() = delete;

    FileTapeFactory(Config config) {
        this->config = config;
    }

    AbstractTape<T> &create() override {
        std::string filePath = generateFilePath();
        return FileTape<T>(filePath,
                           config.readDataDelay,
                           config.writeDataDelay,
                           config.moveTapeDelay);
    };

private:
    Config config;
    int fileCounter = 0;

    std::string generateFilePath() {
        std::stringstream ss;
        ss << config.tempDirectoryPath << fileCounter;
        ++fileCounter;
        return ss.str();
    }
};
