add_executable(${PROJECT_NAME} main.cpp
        ../lib/AbstractTapeFactory.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})