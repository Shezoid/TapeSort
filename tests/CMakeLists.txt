include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        tape-sort-lib-tests
        SorterTests.cpp
        ../lib/tapes/factories/FileTapeFactory.h
        FileTapeTests.cpp
)

target_link_libraries(
        tape-sort-lib-tests
        GTest::gtest_main
        GTest::gmock_main
)

target_compile_definitions(tape-sort-lib-tests PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

target_include_directories(tape-sort-lib-tests PUBLIC ${PROJECT_SOURCE_DIR})

target_include_directories(tape-sort-lib-tests PUBLIC ${PROJECT_SOURCE_DIR}/lib)

include(GoogleTest)

gtest_discover_tests(tape-sort-lib-tests)