add_library(starjumper STATIC
        dice_throw.h
        dice_throw.c
        hex_coordinate.h
        hex_coordinate.c
        starjumper.h
        string_array.h
        string_array.c
        string_join.h
        string_join.c
        subsector.h
        subsector.c
        trade_classification.h
        trade_classification.c
        world.h
        world.c
)
target_include_directories(starjumper
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
target_link_libraries(starjumper
        PUBLIC rnd
        PUBLIC xmalloc
)

add_executable(starjumper_tests
        starjumper_tests.c
)
target_link_libraries(starjumper_tests starjumper)
add_test(starjumper_tests starjumper_tests)
