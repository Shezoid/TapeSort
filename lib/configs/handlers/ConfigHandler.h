#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <utility>

#include "BaseHandler.h"
#include "../Config.h"


template<typename T>
class ConfigHandler : public AbstractConfigHandler {
public:
    ConfigHandler() = delete;

    ConfigHandler(std::string key, std::function<void(Config::Builder &, T)> function) {
        this->key = std::move(key);
        this->function = function;
    }

    Config::Builder& handle(
        std::unordered_map<std::string, std::string> &configs,
        Config::Builder& builder) override {
        T value(configs[key]);
        function(builder, value);
        if (next != nullptr) {
            return next->handle(configs,builder);
        }
        return builder;
    }

private:
    std::string key;
    std::function<void(Config::Builder &, T)> function;
};

template<std::integral T>
class ConfigHandler<T> : public AbstractConfigHandler {
public:
    ConfigHandler() = delete;

    ConfigHandler(std::string key, std::function<void(Config::Builder &, T)> function) {
        this->key = std::move(key);
        this->function = function;
    }

    Config::Builder& handle(
        std::unordered_map<std::string, std::string> &configs,
        Config::Builder& builder) override {
        T value = std::stoll(configs[key]);
        function(builder, value);
        if (next != nullptr) {
            return next->handle(configs,builder);
        }
        return builder;
    }

private:
    std::string key;
    std::function<void(Config::Builder &, T)> function;
};

template<>
class ConfigHandler<std::chrono::milliseconds> : public AbstractConfigHandler {
public:
    ConfigHandler() = delete;

    ConfigHandler(std::string key, std::function<void(Config::Builder &, std::chrono::milliseconds)> function) {
        this->key = std::move(key);
        this->function = std::move(function);
    }

    Config::Builder& handle(
        std::unordered_map<std::string, std::string> &configs,
        Config::Builder& builder) override {
        std::chrono::milliseconds value(std::stoll(configs[key]));
        function(builder, value);
        if (next != nullptr) {
            return next->handle(configs, builder);
        }
        return builder;
    }

private:
    std::string key;
    std::function<void(Config::Builder &, std::chrono::milliseconds)> function;
};
