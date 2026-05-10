add_library(
        tape-sort
        FileTape.h
        AbstractTape.h
        Config.h
        TapeSorter.h
        FileTapeFactory.h
)
target_include_directories(tape-sort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})