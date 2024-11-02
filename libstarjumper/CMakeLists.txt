add_library(starjumper STATIC
        alloc_or_die.c
        rnd.c
        starjumper/sj_dice_throw.c
        starjumper/sj_hex_coordinate.c
        starjumper/sj_string_array.c
        starjumper/sj_string.c
        starjumper/sj_subsector.c
        starjumper/sj_trade_classification.c
        starjumper/sj_world.c
)
target_include_directories(starjumper
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/"
)

add_executable(starjumper_tests
        starjumper/starjumper_tests.c
)
target_link_libraries(starjumper_tests starjumper)
add_test(starjumper_tests starjumper_tests)
