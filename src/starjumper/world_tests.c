#include <starjumper/starjumper.h>

#include <checks.h>
#include <rnd.h>
#include <stdlib.h>


static void
test_world_alloc_for_minimum_rolls(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(0);
    struct sj_world *world = sj_world_alloc("Test", sj_hex_coordinate_make(1, 2), rnd);

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
    rnd_free(rnd);
}


int
main(int argc, char *argv[])
{
    test_world_alloc_for_minimum_rolls();
    return EXIT_SUCCESS;
}
