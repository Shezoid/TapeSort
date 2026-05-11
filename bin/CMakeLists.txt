add_executable(${PROJECT_NAME} main.cpp)

target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})