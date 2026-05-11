#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "Config.h"

class Handler {
public:
    virtual ~Handler() = default;

    virtual Config::Builder handle(
        std::unordered_map<std::string, std::string> &configs,
        Config::Builder builder) = 0;

    virtual Handler *setNext(Handler *) = 0;
};


class AbstractConfigHandler : public Handler {
public:
    AbstractConfigHandler() : next(nullptr) {
    }

    ~AbstractConfigHandler() override {
        if (next != nullptr) {
            delete next;
        }
    }

    Handler *setNext(Handler *handler) override {
        if (next != nullptr) {
            this->next->setNext(handler);
        } else {
            this->next = handler;
        }
        return this;
    }

private:
    Handler *next = 0;
};


template<typename T>
class ConfigHandler : public AbstractConfigHandler {
public:
    ConfigHandler() = delete;

    ConfigHandler(std::string key, std::function<void(Config::Builder&, T)> function) {
        this->key = key;
        this->function = function;
    }

    Config::Builder handle(
        std::unordered_map<std::string, std::string> &configs,
        Config::Builder builder) override {
        T value(configs[key]);
        function(builder, value);
        return builder;
    }

private:
    std::string key;
    std::function<void(Config::Builder&, T)> function;
};

template<std::integral T>
class ConfigHandler<T> : public AbstractConfigHandler {
public:
    ConfigHandler() = delete;

    ConfigHandler(std::string key, std::function<void(Config::Builder&, T)> function) {
        this->key = key;
        this->function = function;
    }

    Config::Builder handle(
        std::unordered_map<std::string, std::string> &configs,
        Config::Builder builder) override {
        T value = std::stoll(configs[key]);
        function(builder, value);
        return builder;
    }

private:
    std::string key;
    std::function<void(Config::Builder&, T)> function;
};

template<>
class ConfigHandler<std::chrono::milliseconds> : public AbstractConfigHandler {
public:
    ConfigHandler() = delete;

    ConfigHandler(std::string key, std::function<void(Config::Builder&, std::chrono::milliseconds)> function) {
        this->key = key;
        this->function = function;
    }

    Config::Builder handle(
        std::unordered_map<std::string, std::string> &configs,
        Config::Builder builder) override {
        std::chrono::milliseconds value(std::stoll(configs[key]));
        function(builder, value);
        return builder;
    }

private:
    std::string key;
    std::function<void(Config::Builder&, std::chrono::milliseconds)> function;
};