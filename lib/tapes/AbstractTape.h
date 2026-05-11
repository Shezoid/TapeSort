#pragma once
#include <concepts>


template<std::integral T>
class AbstractTape {
public:
    virtual ~AbstractTape() = default;

    virtual void convertFromStream(std::istream& stream) = 0;

    virtual T read() = 0;

    virtual void write(T value) = 0;

    virtual void next() = 0;

    virtual void previous() = 0;

    virtual std::size_t getLength()= 0;

    virtual bool hasNext() = 0;

    virtual bool hasPrevious() = 0;
};
