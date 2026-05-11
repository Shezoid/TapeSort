#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "InMemoryTapeAdapterFactory.h"
#include "TapeSorter.h"


TEST(TapeSorterTesting, SimpleSorterTest) {
    // Arrange
    const std::size_t maxMemoryCapacity = 100;
    AbstractTapeFactory<int32_t>* factory = new InMemoryTapeAdapterFactory<int32_t>(maxMemoryCapacity);
    std::vector<int32_t> inputValues = {8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<int32_t> expected = {1, 2, 3, 4, 5, 6, 7, 8};
    AbstractTape<int32_t>* inputTape = new InMemoryTapeAdapter<int32_t>(inputValues);
    AbstractTape<int32_t>* outputTape = factory->create();
    TapeSorter<int32_t> sorter(inputTape, outputTape, factory, maxMemoryCapacity);

    // Act
    sorter.sort();
    while (outputTape->hasPrevious()) {
        outputTape->previous();
    }

    // Assert
    for (int i = 0; i < inputValues.size(); ++i) {
        ASSERT_EQ(outputTape->read(), expected[i]);
        outputTape->next();
    }

    delete inputTape;
    delete outputTape;
    delete factory;
}

TEST(TapeSorterTesting, SimpleSorterWithMinimumMemoryTest) {
    // Arrange
    const std::size_t maxMemoryCapacity = 16;
    AbstractTapeFactory<int32_t>* factory = new InMemoryTapeAdapterFactory<int32_t>(maxMemoryCapacity);
    std::vector<int32_t> inputValues = {8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<int32_t> expected = {1, 2, 3, 4, 5, 6, 7, 8};
    AbstractTape<int32_t>* inputTape = new InMemoryTapeAdapter<int32_t>(inputValues);
    AbstractTape<int32_t>* outputTape = factory->create();
    TapeSorter<int32_t> sorter(inputTape, outputTape, factory, maxMemoryCapacity);

    // Act
    sorter.sort();
    while (outputTape->hasPrevious()) {
        outputTape->previous();
    }

    // Assert
    for (int i = 0; i < inputValues.size(); ++i) {
        ASSERT_EQ(outputTape->read(), expected[i]);
        outputTape->next();
    }

    delete inputTape;
    delete outputTape;
    delete factory;
}
