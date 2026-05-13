#pragma once

#include <concepts>
#include <queue>
#include <utility>

template<std::integral T>
class TapeSorter {
public:
    TapeSorter(std::shared_ptr<AbstractTape<T> > inputTape,
               std::shared_ptr<AbstractTape<T> > outputTape,
               std::shared_ptr<AbstractTapeFactory<T> > factory,
               const size_t maxMemoryCapacity) {
        this->inputTape = inputTape;
        this->outputTape = outputTape;
        this->factory = factory;
        maxCapacity = maxMemoryCapacity / sizeof(T);
        temp.resize(maxCapacity / 2);
    }

    void sort() {
        std::vector<std::shared_ptr<AbstractTape<T>> > tapes
                = std::move(getTapes());

        auto comparator = [](const HeapNode &t1, const HeapNode &t2) {
            if (t1.value != t2.value) {
                return t1.value > t2.value;
            }
            return t1.tapeIndex > t2.tapeIndex;
        };
        std::priority_queue<HeapNode, std::vector<HeapNode>, decltype(comparator)> heap;
        for (std::size_t i = 0; i < tapes.size(); ++i) {
            HeapNode heapNode(tapes[i]->read(), i);

            heap.push(heapNode);
        }

        std::size_t length = inputTape->getLength();
        for (std::size_t i = 0; i < length; ++i) {
            HeapNode heapNode = heap.top();
            heap.pop();
            outputTape->write(heapNode.value);
            outputTape->next();
            if (tapes[heapNode.tapeIndex]->hasPrevious()) {
                tapes[heapNode.tapeIndex]->previous();
                heapNode = {tapes[heapNode.tapeIndex]->read(), heapNode.tapeIndex};
                heap.push(std::move(heapNode));
            }
        }
    }


    std::vector<std::shared_ptr<AbstractTape<T>> > getTapes() {
        std::vector<std::shared_ptr<AbstractTape<T>> > tapes;
        std::vector<T> elements(maxCapacity / 2);

        std::size_t i = 0;
        std::size_t length = 0;

        while (inputTape->hasNext()) {
            if (i >= maxCapacity / 2) {
                std::shared_ptr<AbstractTape<T>> tape = factory->create();

                mergeSort(0, maxCapacity / 2, elements);
                // std::stable_sort(elements.begin(), elements.end());

                auto begin = elements.rbegin();
                while (begin != elements.rend()) {
                    tape->write(*begin);
                    tape->next();
                    ++begin;
                }
                tape->previous();
                tapes.push_back(tape);
                length = 0;
                i = 0;
            }
            elements[i] = inputTape->read();
            inputTape->next();
            ++i;
            ++length;
        }

        if (!elements.empty()) {
            std::shared_ptr<AbstractTape<T>> tape = factory->create();
            auto end = elements.begin() + length;

            mergeSort(0, length, elements);
            // std::stable_sort(elements.begin(), end);

            for (int i = length - 1; i >= 0; --i) {
                tape->write(elements[i]);
                tape->next();
            }
            tape->previous();
            tapes.push_back(tape);
        }

        return tapes;
    }

private:
    std::shared_ptr<AbstractTape<T> > inputTape;
    std::shared_ptr<AbstractTape<T> > outputTape;
    std::shared_ptr<AbstractTapeFactory<T> > factory;
    size_t maxCapacity;
    std::vector<T> temp;

    
    void merge(std::vector<T>& arr, const std::size_t& left, const std::size_t& mid, const std::size_t& right) {
        std::size_t leftIterator = 0;
        std::size_t rightIterator = 0;

        while (((left + leftIterator) < mid) && ((mid + rightIterator) < right)) {
            if (arr[left + leftIterator] < arr[mid + rightIterator]) {
                temp[leftIterator + rightIterator] = arr[left + leftIterator];
                leftIterator++;
            }
            else {
                temp[leftIterator + rightIterator] = arr[mid + rightIterator];
                rightIterator++;
            }
        }
        while ((left + leftIterator) < mid) {
            temp[leftIterator + rightIterator] = arr[left + leftIterator];
            leftIterator++;
        }

        while ((mid + rightIterator) < right) {
            temp[leftIterator + rightIterator] = arr[mid + rightIterator];
            rightIterator++;
        }
        for (int i = 0; i < (leftIterator + rightIterator); i++) {
            arr[left + i] = temp[i];
        }
    }


    void mergeSort(std::size_t left, std::size_t right, std::vector<T>& arr) {
        if (left + 1 >= right) {
            return;
        }

        std::size_t mid = (left + right) / 2;
        mergeSort(left, mid, arr);
        mergeSort(mid, right, arr);
        merge(arr, left, mid, right);
    }


    struct HeapNode {
        HeapNode(T value, const std::size_t tapeIndex) : value(value), tapeIndex(tapeIndex) {
        }

        T value;
        std::size_t tapeIndex;
    };
};
