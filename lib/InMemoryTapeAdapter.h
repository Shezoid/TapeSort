#pragma once

#include <concepts>
#include <vector>

#include "AbstractTape.h"

template<std::integral T>
class InMemoryTapeAdapter : public AbstractTape<T> {
public:
    InMemoryTapeAdapter(std::vector<T>& storage) : storage(storage) {
    }

    InMemoryTapeAdapter() = delete;

    InMemoryTapeAdapter(const std::vector<T>& storage) {
        auto begin = storage.begin();
        while (begin != storage.end()) {
            storage.push_back(*begin);
            ++begin;
        }
    }

    InMemoryTapeAdapter(const InMemoryTapeAdapter& other) : storage(other.storage) {}

    InMemoryTapeAdapter(InMemoryTapeAdapter&& other) noexcept : storage(other.storage) {}

    InMemoryTapeAdapter& operator=(const InMemoryTapeAdapter& other) {
        if (this != &other ) {
            storage = other.storage;
            current = other.current;
        }
        return *this;
    }

    InMemoryTapeAdapter& operator=(InMemoryTapeAdapter&& other) noexcept {
        if (this != &other ) {
            storage = std::move(other.storage);
            current = other.current;
        }
        return *this;
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
        return storage[current];
    }

    void write(T value) override {
        storage[current] = value;
    }

    void next() override {
        if (current < storage.size()) ++current;
    }

    void previous() override {
        if (current > 0) --current;
    }

    std::size_t getLength() override {
        return storage.size();
    }

    bool hasNext() override {
        return current < storage.size();
    }

    bool hasPrevious() override {
        return current > 0;
    }

private:
    std::vector<T> storage;
    std::size_t current = 0;
};
