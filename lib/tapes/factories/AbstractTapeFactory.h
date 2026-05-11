#pragma once

#include <string>
#include "../AbstractTape.h"
#include "concepts"

template<std::integral T>
class AbstractTapeFactory {
public:
    virtual std::shared_ptr<AbstractTape<T>> create() = 0;

    virtual ~AbstractTapeFactory() = default;
};
