#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../lib/tapes/factories/InMemoryTapeAdapterFactory.h"
#include "TapeSorter.h"


TEST(TapeSorterTesting, SimpleSorterTest) {
    // Arrange
    const std::size_t maxMemoryCapacity = 100;
    std::shared_ptr<AbstractTapeFactory<int32_t> > factory
            = std::make_shared<InMemoryTapeAdapterFactory<int32_t> >(maxMemoryCapacity);
    std::vector<int32_t> inputValues = {8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<int32_t> expected = {1, 2, 3, 4, 5, 6, 7, 8};
    std::shared_ptr<AbstractTape<int32_t> > inputTape = std::make_shared<InMemoryTapeAdapter<int32_t> >(inputValues);
    std::shared_ptr<AbstractTape<int32_t> > outputTape = factory->create();
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
}

TEST(TapeSorterTesting, SimpleSorterWithMinimumMemoryTest) {
    // Arrange
    const std::size_t maxMemoryCapacity = 16;
    std::shared_ptr<AbstractTapeFactory<int32_t> > factory
            = std::make_shared<InMemoryTapeAdapterFactory<int32_t> >(maxMemoryCapacity);
    std::vector<int32_t> inputValues = {8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<int32_t> expected = {1, 2, 3, 4, 5, 6, 7, 8};
    std::shared_ptr<AbstractTape<int32_t> > inputTape =
            std::make_shared<InMemoryTapeAdapter<int32_t> >(inputValues);
    std::shared_ptr<AbstractTape<int32_t> > outputTape = factory->create();
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
}
