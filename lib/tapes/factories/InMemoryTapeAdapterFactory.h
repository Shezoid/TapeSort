#pragma once
#include "AbstractTapeFactory.h"
#include "../InMemoryTapeAdapter.h"

template<std::integral T>
class InMemoryTapeAdapterFactory : public AbstractTapeFactory<T> {
public:
    InMemoryTapeAdapterFactory() = delete;

    InMemoryTapeAdapterFactory(std::size_t maxCapacity) {
        this->maxCapacity = maxCapacity;
    }

    std::shared_ptr<AbstractTape<T> > create() override {
        std::vector<T> storage(maxCapacity);
        return std::make_shared<InMemoryTapeAdapter<T> >(storage);
    }

private:
    std::size_t maxCapacity;
};
