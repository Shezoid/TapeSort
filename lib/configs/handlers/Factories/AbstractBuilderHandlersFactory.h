#pragma once
#include "../ConfigHandler.h"


class AbstractBuilderHandlersFactory {
public:
    virtual ~AbstractBuilderHandlersFactory() = default;

    virtual std::unique_ptr<Handler> createHandlersChain() = 0;
};
