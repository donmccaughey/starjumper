add_library(starjumper STATIC
        starjumper/sj_dice_throw.h
        starjumper/sj_dice_throw.c
        starjumper/sj_hex_coordinate.h
        starjumper/sj_hex_coordinate.c
        starjumper/sj_string.h
        starjumper/sj_string.c
        starjumper/sj_string_array.h
        starjumper/sj_string_array.c
        starjumper/sj_subsector.h
        starjumper/sj_subsector.c
        starjumper/sj_trade_classification.h
        starjumper/sj_trade_classification.c
        starjumper/sj_world.h
        starjumper/sj_world.c
        starjumper/starjumper.h
)
target_include_directories(starjumper
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/"
)
target_link_libraries(starjumper
        PUBLIC alloc_or_die
        PUBLIC rnd
)

add_executable(starjumper_tests
        starjumper/starjumper_tests.c
)
target_link_libraries(starjumper_tests starjumper)
add_test(starjumper_tests starjumper_tests)
