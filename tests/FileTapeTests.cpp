#include <filesystem>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../lib/tapes/AbstractTape.h"
#include "../lib/tapes/FileTape.h"

TEST(FileTapeTesting, SimpleTapeTest) {
    // Arrange
    std::filesystem::path testPath =
            std::filesystem::path(PROJECT_ROOT) / "tmp" / "SimpleTapeTest";
    const std::vector<int32_t> expected = {1, 2, 3, 4, 5, 6, 7, 8};
    std::shared_ptr<AbstractTape<int32_t> > tape
            = std::make_shared<FileTape<int32_t> >(testPath,
                                                   std::chrono::milliseconds(0),
                                                   std::chrono::milliseconds(0),
                                                   std::chrono::milliseconds(0)
            );

    // Act
    for (int32_t i = 1; i < expected.size() + 1; ++i) {
        tape->write(i);
        tape->next();
    }
    while (tape->hasPrevious()) {
        tape->previous();
    }

    // Assert
    for (int i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], tape->read());
        tape->next();
    }

    std::filesystem::remove(testPath);
}


TEST(FileTapeTesting, TapeTestWithOverridingData) {
    // Arrange
    std::filesystem::path testPath =
            std::filesystem::path(PROJECT_ROOT) / "tmp" / "SimpleTapeWithOverridingDataTest";

    const std::vector<int32_t> expected = {5, 6, 7, 8};
    std::shared_ptr<AbstractTape<int32_t> > tape
            = std::make_shared<FileTape<int32_t> >(testPath,
                                                   std::chrono::milliseconds(0),
                                                   std::chrono::milliseconds(0),
                                                   std::chrono::milliseconds(0));

    // Act
    for (int32_t i = 1; i < 9; ++i) {
        tape->write(i);
        tape->next();
        if (i == 4) {
            for (int k = 0; k < 4; ++k) {
                tape->previous();
            }
        }
    }
    while (tape->hasPrevious()) {
        tape->previous();
    }

    // Assert
    for (int i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], tape->read());
        tape->next();
    }

    std::filesystem::remove(testPath);
}
