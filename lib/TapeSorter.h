#pragma once

#include "FileTapeFactory.h"
#include <concepts>
#include <functional>
#include <queue>
#include <utility>

template<std::integral T>
class TapeSorter {
public:
    TapeSorter(AbstractTape<T> inputTape,
               AbstractTape<T> outputTape,
               AbstractTapeFactory<T> factory,
               const size_t maxMemoryCapacity) {
        this->inputTape = inputTape;
        this->outputTape = outputTape;
        this->factory = factory;
        maxCapacity = maxMemoryCapacity / sizeof(T);
    }

    void sort() {
        std::vector<AbstractTape<T> > tapes
                = std::move(getTapes());

        auto comparator = [](const HeapNode &t1, const HeapNode &t2) {
            if (t1.value != t2.value) {
                return t1.value > t2.value;
            }
            return t1.tapeIndex > t2.tapeIndex;
        };
        std::priority_queue<HeapNode, std::vector<HeapNode>, decltype(comparator) > heap;
        for (std::size_t i = 0; i < tapes.size(); ++i) {
            heap.push(std::move({tapes[i].read, i}));
        }

        for (std::size_t i = 0; i < inputTape.getLength(); ++i) {
            HeapNode heapNode = heap.top();
            heap.pop();
            outputTape.write(heapNode.value);

            if (tapes[heapNode.tapeIndex].hasPrevious()) {
                tapes[heapNode.tapeIndex].previous();
                heapNode = {tapes[heapNode.tapeIndex].read(), heapNode.tapeIndex};
                heap.push(std::move(heapNode));
            }
        }

    }


    std::vector<AbstractTape<T> > getTapes() {
        std::vector<AbstractTape<T> > tapes;
        std::vector<T> elements(maxCapacity);

        size_t i = 0;

        while (inputTape.hasNext()) {
            if (i >= maxCapacity) {
                AbstractTape<T> tape = factory.create();
                std::stable_sort(elements.begin(), elements.end());
                auto begin = elements.rbegin();
                while (begin != elements.rend()) {
                    tape.write(*begin);
                    tape.next();
                    ++begin;
                }
                tapes.push_back(std::move(tape));
                elements.clear();
            }
            elements[i] = inputTape.read();
            inputTape.next();
            ++i;
        }

        return tapes;
    }

private:
    AbstractTape<T> inputTape;
    AbstractTape<T> outputTape;
    AbstractTapeFactory<T> factory;
    size_t maxCapacity;

    struct HeapNode {
        T value;
        std::size_t tapeIndex;
    };
};
