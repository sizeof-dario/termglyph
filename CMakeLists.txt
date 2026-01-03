cmake_minimum_required(VERSION 3.28.3)

project(termglyph)

add_library(termglyph)
add_executable(termglyph_testing)

target_sources(termglyph
    PRIVATE
        src/print.c

    PUBLIC
        FILE_SET HEADERS 
        BASE_DIRS
            include
        FILES
            include/termglyph.h
)

target_sources(termglyph_testing
    PRIVATE
        main.c
)

target_link_libraries(termglyph_testing
    PRIVATE
        termglyph
)