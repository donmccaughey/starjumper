add_library(alloc_or_die STATIC
        alloc_or_die.h
        alloc_or_die.c
)
target_include_directories(alloc_or_die
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)
