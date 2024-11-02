add_library(starjumper STATIC
        sj_dice_throw.h
        sj_dice_throw.c
        sj_hex_coordinate.h
        sj_hex_coordinate.c
        sj_string_array.h
        sj_string_array.c
        sj_string_join.h
        sj_string_join.c
        sj_subsector.h
        sj_subsector.c
        sj_trade_classification.h
        sj_trade_classification.c
        sj_world.h
        sj_world.c
        starjumper.h
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
