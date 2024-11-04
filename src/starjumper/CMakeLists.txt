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


add_executable(dice_throw_tests
        dice_throw_tests.c
)
target_link_libraries(dice_throw_tests
        checks
        starjumper
)
add_test(dice_throw_tests dice_throw_tests)


add_executable(hex_coordinate_tests
        hex_coordinate_tests.c
)
target_link_libraries(hex_coordinate_tests
        checks
        starjumper
)
add_test(hex_coordinate_tests hex_coordinate_tests)


add_executable(world_tests
        world_tests.c
)
target_link_libraries(world_tests
        checks
        starjumper
)
add_test(world_tests world_tests)
