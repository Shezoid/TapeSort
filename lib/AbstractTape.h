#pragma once
#include <concepts>


template<std::integral T>
class AbstractTape {
public:
    virtual ~AbstractTape() = default;

    virtual T read() = 0;

    virtual void write(T value) = 0;

    virtual void next() = 0;

    virtual void previous() = 0;

    virtual std::size_t getLength() = 0;
};
