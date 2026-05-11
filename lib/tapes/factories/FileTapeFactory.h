#pragma once
#include <concepts>
#include <utility>

#include "../../configs/Config.h"
#include "../FileTape.h"
#include "AbstractTapeFactory.h"


template<std::integral T>
class FileTapeFactory : public AbstractTapeFactory<T> {
public:
    FileTapeFactory() = delete;

    FileTapeFactory(Config config) : config(config) {
        this->config = std::move(config);
    }

    std::shared_ptr<AbstractTape<T> > create() override {
        std::string filePath = generateFilePath();
        return std::make_shared<FileTape<T> >(filePath,
                                              config.readDataDelay,
                                              config.writeDataDelay,
                                              config.moveTapeDelay,
                                              true);
    };

private:
    Config config;
    int fileCounter = 0;

    std::string generateFilePath() {
        std::stringstream ss;
        ss << config.tempDirectoryPath << '/' << fileCounter;
        ++fileCounter;
        return ss.str();
    }
};
