#pragma once

#include "AbstactHandler.h"

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
