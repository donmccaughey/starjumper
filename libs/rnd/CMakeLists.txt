add_library(rnd STATIC
        rnd.h
        rnd.c
)
target_include_directories(rnd
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)
