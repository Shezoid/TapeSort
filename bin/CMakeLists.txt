add_executable(${PROJECT_NAME} main.cpp
        ../lib/AbstractTapeFactory.h
        ../lib/InMemoryTapeAdapter.h
        ../lib/InMemoryTapeAdapterFactory.h
        ../lib/ConfigBuilderHandlers.h
        ../lib/EnvParser.h
        ../lib/ConfigBuilderHandlersFactory.h
        ../lib/AbstractBuilderHandlersFactory.h
)

target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})