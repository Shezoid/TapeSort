#pragma once
#include <unordered_map>

#include "configs/Config.h"

class Handler {
public:
    virtual ~Handler() = default;

    virtual Config::Builder handle(
        std::unordered_map<std::string, std::string> &configs,
        Config::Builder builder) = 0;

    virtual Handler *setNext(Handler *) = 0;
};
