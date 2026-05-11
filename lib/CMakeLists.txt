add_library(
        tape-sort
        tapes/FileTape.h
        tapes/AbstractTape.h
        configs/Config.h
        TapeSorter.h
        tapes/factories/FileTapeFactory.h
)
target_include_directories(tape-sort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})