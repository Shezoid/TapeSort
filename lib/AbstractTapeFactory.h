#pragma once

#include <string>
#include "AbstractTape.h"
#include "concepts"

template<std::integral T>
class AbstractTapeFactory {
public:
    virtual AbstractTape<T>& create() = 0;

    virtual ~AbstractTapeFactory();
};
