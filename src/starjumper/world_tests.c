#include <starjumper/starjumper.h>

#include <checks.h>
#include <dice.h>
#include <stdlib.h>


static void
test_world_alloc_for_minimum_rolls(void)
{
    int die_value = 0;
    struct die die = die_make_fixed(&die_value);
    struct sj_world *world = sj_world_alloc("Test", sj_hex_coordinate_make(1, 2), die);

    check_not_null(world);
    check_str_eq(world->name, "Test");
    check_int_eq(world->hex_coordinate.horizontal, 1);
    check_int_eq(world->hex_coordinate.vertical, 2);

    check_int_eq(world->starport, 'A');
    check_false(world->naval_base);
    check_false(world->scout_base);
    check_true(world->gas_giant);

    check_int_eq(world->size, 0);
    check_int_eq(world->atmosphere, 0);
    check_int_eq(world->hydrographics, 0);
    check_int_eq(world->population, 0);
    check_int_eq(world->government, 0);
    check_int_eq(world->law_level, 0);

    check_int_eq(world->tech_level, 0);

    check_int_eq(world->trade_classifications_count, 5);
    check_str_eq(world->trade_classifications[0]->name, "Asteroid Belt");
    check_str_eq(world->trade_classifications[1]->name, "Barren World");
    check_str_eq(world->trade_classifications[2]->name, "Low Population");
    check_str_eq(world->trade_classifications[3]->name, "Non-industrial");
    check_str_eq(world->trade_classifications[4]->name, "Vacuum World");

    sj_world_free(world);
}


static void
test_world_alloc_for_maximum_rolls(void)
{
    int die_value = 6;
    struct die die = die_make_fixed(&die_value);
    struct sj_world *world = sj_world_alloc("Test", sj_hex_coordinate_make(8, 10), die);

    check_not_null(world);
    check_str_eq(world->name, "Test");
    check_int_eq(world->hex_coordinate.horizontal, 8);
    check_int_eq(world->hex_coordinate.vertical, 10);

    check_int_eq(world->starport, 'X');
    check_false(world->naval_base);
    check_false(world->scout_base);
    check_false(world->gas_giant);

    check_int_eq(world->size, 10);
    check_int_eq(world->atmosphere, 15);
    check_int_eq(world->hydrographics, 10);
    check_int_eq(world->population, 10);
    check_int_eq(world->government, 15);
    check_int_eq(world->law_level, 20);

    check_int_eq(world->tech_level, 8);

    check_int_eq(world->trade_classifications_count, 3);
    check_str_eq(world->trade_classifications[0]->name, "Fluid Oceans");
    check_str_eq(world->trade_classifications[1]->name, "High Population");
    check_str_eq(world->trade_classifications[2]->name, "Water World");

    sj_world_free(world);
}


static void
test_world_alloc_for_average_rolls(void)
{
    int die_value = 3;
    struct die die = die_make_fixed(&die_value);
    struct sj_world *world = sj_world_alloc("Test", sj_hex_coordinate_make(2, 3), die);

    check_not_null(world);
    check_str_eq(world->name, "Test");
    check_int_eq(world->hex_coordinate.horizontal, 2);
    check_int_eq(world->hex_coordinate.vertical, 3);

    check_int_eq(world->starport, 'B');
    check_false(world->naval_base);
    check_false(world->scout_base);
    check_true(world->gas_giant);

    check_int_eq(world->size, 4);
    check_int_eq(world->atmosphere, 3);
    check_int_eq(world->hydrographics, 2);
    check_int_eq(world->population, 4);
    check_int_eq(world->government, 3);
    check_int_eq(world->law_level, 2);

    check_int_eq(world->tech_level, 10);

    check_int_eq(world->trade_classifications_count, 2);
    check_str_eq(world->trade_classifications[0]->name, "Non-industrial");
    check_str_eq(world->trade_classifications[1]->name, "Poor");

    sj_world_free(world);
}


static void
test_string_from_world(void)
{
    int die_value = 3;
    struct die die = die_make_fixed(&die_value);
    struct sj_world *world = sj_world_alloc("Test", sj_hex_coordinate_make(3, 7), die);
    char *string = sj_string_from_world(world);

    check_str_eq(string, "Test               0307 B432432-A   Non-industrial. Poor.                     G");

    free(string);
    sj_world_free(world);
}


int
main(int argc, char *argv[])
{
    test_world_alloc_for_minimum_rolls();
    test_world_alloc_for_maximum_rolls();
    test_world_alloc_for_average_rolls();

    test_string_from_world();
    return EXIT_SUCCESS;
}
