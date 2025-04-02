#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "dice.h"


static void
test_high_die(void)
{
    for (int i = 0; i < 10; ++i) {
        int roll = roll_die(6, high_die);
        assert(6 == roll);
    }
}


static void
test_low_die(void)
{
    for (int i = 0; i < 10; ++i) {
        int roll = roll_die(20, low_die);
        assert(1 == roll);
    }
}


static void
test_median_die(void)
{
    for (int i = 0; i < 10; ++i) {
        int roll = roll_die(12, median_die);
        assert(7 == roll);
    }
}


static void
test_random_die(void)
{
    for (int i = 0; i < 10; ++i) {
        int roll = roll_die(8, random_die);
        assert(1 <= roll);
        assert(8 >= roll);
    }
}


static void
test_ascending_die(void)
{
    int initial_value = 1;
    struct die die = die_make_ascending(&initial_value);

    for (int i = 0; i < 10; ++i) {
        int roll = roll_die(12, die);
        int expected = i + 1;
        assert(expected == roll);
    }
}


static void
test_ascending_die_too_low(void)
{
    int initial_value = 0;
    struct die die = die_make_ascending(&initial_value);

    int roll = roll_die(12, die);
    assert(1 == roll);

    roll = roll_die(12, die);
    assert(1 == roll);

    roll = roll_die(12, die);
    assert(2 == roll);

    roll = roll_die(12, die);
    assert(3 == roll);
}


static void
test_ascending_die_too_high(void)
{
    int initial_value = 3;
    struct die die = die_make_ascending(&initial_value);

    int roll = roll_die(4, die);
    assert(3 == roll);

    roll = roll_die(4, die);
    assert(4 == roll);

    roll = roll_die(4, die);
    assert(1 == roll);

    roll = roll_die(4, die);
    assert(2 == roll);

    roll = roll_die(4, die);
    assert(3 == roll);
}


static void
test_fixed_die(void)
{
    int fixed_value = 3;
    struct die die = die_make_fixed(&fixed_value);

    for (int i = 0; i < 10; ++i) {
        int roll = roll_die(12, die);
        assert(3 == roll);
    }
}


static void
test_fixed_die_too_low(void)
{
    int fixed_value = 0;
    struct die die = die_make_fixed(&fixed_value);

    int roll = roll_die(6, die);
    assert(1 == roll);
}


static void
test_fixed_die_too_high(void)
{
    int fixed_value = 7;
    struct die die = die_make_fixed(&fixed_value);

    int roll = roll_die(6, die);
    assert(6 == roll);
}


static void
test_mod_make(void)
{
    struct mod mod = mod_make('+', 3);
    assert('+' == mod.op);
    assert(3 == mod.value);
}


static void
test_mod_make_normalizes_negative_values(void)
{
    struct mod mod = mod_make('+', -3);
    assert('-' == mod.op);
    assert(3 == mod.value);

    mod = mod_make('-', -3);
    assert('+' == mod.op);
    assert(3 == mod.value);

    mod = mod_make('x', -10);
    assert('x' == mod.op);
    assert(-10 == mod.value);
}


static void
test_dice_alloc(void)
{
    struct dice *dice = dice_alloc(2, 8);

    assert(2 == dice->count);
    assert(8 == dice->sides);
    assert(4 == dice->mods_capacity);
    assert(0 == dice->mods_count);

    free(dice);
}


static void
test_dice_alloc_for_zero_count(void)
{
    struct dice *dice = dice_alloc(0, 8);

    assert(0 == dice->count);
    assert(8 == dice->sides);
    assert(4 == dice->mods_capacity);
    assert(0 == dice->mods_count);

    free(dice);
}


static void
test_dice_alloc_with_mods(void)
{
    struct mod mods[] = {
            mod_make('+', 3),
            mod_make('-', 1),
            mod_make('x', 10),
    };
    int mods_count = sizeof(mods) / sizeof(mods[0]);
    struct dice *dice = dice_alloc_with_mods(2, 8, mods, mods_count);

    assert(2 == dice->count);
    assert(8 == dice->sides);
    assert(3 == dice->mods_capacity);
    assert(3 == dice->mods_count);

    assert('+' == dice->mods[0].op);
    assert(3 == dice->mods[0].value);

    assert('-' == dice->mods[1].op);
    assert(1 == dice->mods[1].value);

    assert('x' == dice->mods[2].op);
    assert(10 == dice->mods[2].value);

    free(dice);
}


static void
test_dice_alloc_with_mods_capacity(void)
{
    struct dice *dice = dice_alloc_with_mods_capacity(2, 8, 8);

    assert(2 == dice->count);
    assert(8 == dice->sides);
    assert(8 == dice->mods_capacity);
    assert(0 == dice->mods_count);

    free(dice);
}


static void
test_dice_alloc_parse(void)
{
    struct dice *dice;

    dice = dice_alloc_parse("D");
    assert(dice);
    assert(1 == dice->count);
    assert(6 == dice->sides);
    assert(0 == dice->mods_count);
    free(dice);

    dice = dice_alloc_parse("D4");
    assert(dice);
    assert(1 == dice->count);
    assert(4 == dice->sides);
    assert(0 == dice->mods_count);
    free(dice);

    dice = dice_alloc_parse("2d");
    assert(dice);
    assert(2 == dice->count);
    assert(6 == dice->sides);
    assert(0 == dice->mods_count);
    free(dice);

    dice = dice_alloc_parse("1D4");
    assert(dice);
    assert(1 == dice->count);
    assert(4 == dice->sides);
    assert(0 == dice->mods_count);
    free(dice);

    dice = dice_alloc_parse("3d6");
    assert(dice);
    assert(3 == dice->count);
    assert(6 == dice->sides);
    assert(0 == dice->mods_count);
    free(dice);

    dice = dice_alloc_parse("d-1");
    assert(dice);
    assert(1 == dice->count);
    assert(6 == dice->sides);
    assert(1 == dice->mods_count);
    assert('-' == dice->mods[0].op);
    assert(1 == dice->mods[0].value);
    free(dice);

    dice = dice_alloc_parse("2d6+6");
    assert(dice);
    assert(2 == dice->count);
    assert(6 == dice->sides);
    assert(1 == dice->mods_count);
    assert('+' == dice->mods[0].op);
    assert(6 == dice->mods[0].value);
    free(dice);

    dice = dice_alloc_parse("3d8-0+0");
    assert(dice);
    assert(3 == dice->count);
    assert(8 == dice->sides);
    assert(2 == dice->mods_count);
    assert('-' == dice->mods[0].op);
    assert(0 == dice->mods[0].value);
    assert('+' == dice->mods[1].op);
    assert(0 == dice->mods[1].value);
    free(dice);

    dice = dice_alloc_parse("2d8+3-1x10");
    assert(dice);
    assert(2 == dice->count);
    assert(8 == dice->sides);
    assert(3 == dice->mods_count);
    assert('+' == dice->mods[0].op);
    assert(3 == dice->mods[0].value);
    assert('-' == dice->mods[1].op);
    assert(1 == dice->mods[1].value);
    assert('x' == dice->mods[2].op);
    assert(10 == dice->mods[2].value);
    free(dice);
}


static void
test_dice_alloc_parse_when_invalid(void)
{
    struct dice *dice;

    dice = dice_alloc_parse(NULL);
    assert( ! dice);

    dice = dice_alloc_parse("");
    assert( ! dice);

    dice = dice_alloc_parse("2");
    assert( ! dice);

    dice = dice_alloc_parse("0d4");
    assert( ! dice);

    dice = dice_alloc_parse("2D1");
    assert( ! dice);

    dice = dice_alloc_parse("-2D6");
    assert( ! dice);

    dice = dice_alloc_parse("3D4+");
    assert( ! dice);

    dice = dice_alloc_parse("3D4+-3");
    assert( ! dice);

    dice = dice_alloc_parse("2D6*10");
    assert( ! dice);

    dice = dice_alloc_parse("2D6x10-");
    assert( ! dice);

    dice = dice_alloc_parse("2D6x-10");
    assert( ! dice);
}


static void
test_dice_alloc_parse_for_overflow(void)
{
    assert(INT_MAX == 2147483647);

    struct dice *dice;

    // count overflow
    dice = dice_alloc_parse("2147483647d2");
    assert(dice);
    assert(INT_MAX == dice->count);
    assert(2 == dice->sides);
    assert(0 == dice->mods_count);
    free(dice);

    // INT_MAX + 1 = 2147483648
    // UB for int that implementations typically wrap to INT_MIN = -2147483648
    dice = dice_alloc_parse("2147483648d2");
    assert( ! dice);

    // UINT_MAX = 4294967295
    // UB for int that implementations typically wrap to -1
    dice = dice_alloc_parse("4294967295d2");
    assert( ! dice);

    // UINT_MAX + 2 = 4294967297
    // UB for int that implementations typically wrap to 1
    // 1 is the lowest valid dice count
    dice = dice_alloc_parse("4294967297d2");
    assert( ! dice);


    // sides overflow
    dice = dice_alloc_parse("2d2147483647");
    assert(dice);
    assert(2 == dice->count);
    assert(INT_MAX == dice->sides);
    assert(0 == dice->mods_count);
    free(dice);

    // INT_MAX + 1 = 2147483648
    // UB for int that implementations typically wrap to INT_MIN = -2147483648
    dice = dice_alloc_parse("2d2147483648");
    assert( ! dice);

    // UINT_MAX = 4294967295
    // UB for int that implementations typically wrap to -1
    dice = dice_alloc_parse("2d4294967295");
    assert( ! dice);

    // UINT_MAX + 3 = 4294967298
    // UB for int that implementations typically wrap to 2
    // 2 is the lowest valid dice sides
    dice = dice_alloc_parse("2d4294967298");
    assert( ! dice);


    // mod value overflow
    dice = dice_alloc_parse("2d6+2147483647");
    assert(dice);
    assert(2 == dice->count);
    assert(6 == dice->sides);
    assert(1 == dice->mods_count);
    assert('+' == dice->mods[0].op);
    assert(INT_MAX == dice->mods[0].value);
    free(dice);

    // INT_MAX + 1 = 2147483648
    // UB for int that implementations typically wrap to INT_MIN = -2147483648
    dice = dice_alloc_parse("2d6+2147483648");
    assert( ! dice);

    // UINT_MAX = 4294967295
    // UB for int that implementations typically wrap to -1
    dice = dice_alloc_parse("2d6+4294967295");
    assert( ! dice);

    // UINT_MAX + 1 = 4294967296
    // UB for int that implementations typically wrap to 0
    // 0 is the lowest valid mod value
    dice = dice_alloc_parse("2d6+4294967296");
    assert( ! dice);
}


static void
test_dice_description_alloc(void)
{
   struct mod mods[] = {
           mod_make('+', 3),
           mod_make('-', 1),
           mod_make('x', 10),
    };
    int mods_count = sizeof(mods) / sizeof(mods[0]);
    struct dice *dice = dice_alloc_with_mods(2, 8, mods, mods_count);

    char *description = dice_description_alloc(dice);
    assert(0 == strcmp("2d8+3-1x10", description));

    free(description);
    free(dice);

    dice = dice_alloc_with_mods(1, 12, NULL, 0);
    description = dice_description_alloc(dice);

    assert(0 == strcmp("1d12", description));

    free(description);
    free(dice);
}


static void
test_dice_realloc_add_mod(void)
{
   struct mod mods[] = {
           mod_make('+', 4),
    };
    int mods_count = sizeof(mods) / sizeof(mods[0]);
    struct dice *dice = dice_alloc_with_mods(2, 8, mods, mods_count);

    assert(1 == dice->mods_capacity);
    assert(1 == dice->mods_count);
    assert(4 == dice->mods[0].value);

    dice = dice_realloc_add_mod(dice, mod_make('+', 5));

    assert(dice->mods_capacity > 2);
    assert(2 == dice->mods_count);
    assert(4 == dice->mods[0].value);
    assert(5 == dice->mods[1].value);

    dice = dice_realloc_add_mod(dice, mod_make('x', 6));

    assert(dice->mods_capacity > 3);
    assert(3 == dice->mods_count);
    assert(4 == dice->mods[0].value);
    assert(5 == dice->mods[1].value);
    assert(6 == dice->mods[2].value);

    free(dice);
}


static void
test_dice_realloc_set_mods_capacity_larger(void)
{
   struct mod mods[] = {
           mod_make('+', 3),
           mod_make('x', 5),
    };
    int mods_count = sizeof(mods) / sizeof(mods[0]);
    struct dice *dice = dice_alloc_with_mods(2, 8, mods, mods_count);

    assert(2 == dice->mods_capacity);
    assert(2 == dice->mods_count);
    assert(3 == dice->mods[0].value);
    assert(5 == dice->mods[1].value);

    dice = dice_realloc_set_mods_capacity(dice, 4);

    assert(4 == dice->mods_capacity);
    assert(2 == dice->mods_count);
    assert(3 == dice->mods[0].value);
    assert(5 == dice->mods[1].value);

    dice = dice_realloc_set_mods_capacity(dice, 1);

    assert(1 == dice->mods_capacity);
    assert(1 == dice->mods_count);
    assert(3 == dice->mods[0].value);

    dice = dice_realloc_set_mods_capacity(dice, 0);

    assert(0 == dice->mods_capacity);
    assert(0 == dice->mods_count);

    free(dice);
}


static void
test_roll_alloc(void)
{
    int initial_value = 2;
    struct die die = die_make_ascending(&initial_value);
    struct dice *dice = dice_alloc(3, 6);
    struct roll *roll = roll_alloc(dice, die);

    assert(dice == roll->dice);
    assert(3 == roll->die_rolls_count);
    assert(2 == roll->die_rolls[0]);
    assert(3 == roll->die_rolls[1]);
    assert(4 == roll->die_rolls[2]);

    free(roll);
    free(dice);
}


static void
test_roll_total(void)
{
    int value = 5;
    struct die die = die_make_fixed(&value);

   struct mod mods[] = {
           mod_make('+', 3),
           mod_make('-', 1),
           mod_make('x', 10),
    };
    int mods_count = sizeof(mods) / sizeof(mods[0]);
    struct dice *dice = dice_alloc_with_mods(2, 8, mods, mods_count);

    struct roll *roll = roll_alloc(dice, die);
    int total = roll_total(roll);

    assert(120 == total);

    free(roll);
    free(dice);
}


static void
test_roll_dice(void)
{
    struct dice *dice = dice_alloc(2, 8);

    for (int i = 0; i < 10; ++i) {
        int total = roll_dice(dice, random_die);
        assert(2 <= total);
        assert(16 >= total);
    }

    free(dice);
}


static void
test_roll_dice_with_zero_count(void)
{
    struct dice *dice = dice_alloc(0, 8);

    for (int i = 0; i < 10; ++i) {
        int total = roll_dice(dice, random_die);
        assert(0 == total);
    }

    free(dice);
}


static void
test_roll(void)
{
    for (int i = 0; i < 10; ++i) {
        int total = roll(2, 10, random_die);
        assert(2 <= total);
        assert(20 >= total);
    }
}


static void
test_roll_with_mod(void)
{
    for (int i = 0; i < 10; ++i) {
        int total = roll_with_mod(2, 8, mod_make('+', 2), random_die);
        assert(4 <= total);
        assert(18 >= total);
    }
}


static void
test_roll_with_mods(void)
{
    struct mod mods[] = {
            mod_make('+', 3),
            mod_make('x', 10),
    };
    int mods_count = sizeof(mods) / sizeof(mods[0]);

    for (int i = 0; i < 10; ++i) {
        int total = roll_with_mods(2, 6, mods, mods_count, random_die);
        assert(50 <= total);
        assert(150 >= total);
    }
}


static void
test_roll_parse(void)
{
    struct {
        char const *dice_expression;
        int min_total;
        int max_total;
    } tests[] = {
            { "2d8", 2, 16 },
            { "2d8+2", 4, 18 },
            { "2d8-2", 0, 14 },
            { "2d8x2", 4, 32 },
            { "2d8+2-1x10", 30, 170 },
    };
    int tests_count = sizeof tests / sizeof tests[0];

    for (int i = 0; i < tests_count; ++i) {
        for (int j = 0; j < 10; ++j) {
            int total = roll_parse(tests[i].dice_expression, random_die);
            assert(tests[i].min_total <= total);
            assert(tests[i].max_total >= total);
        }
    }
}


int
main(int argc, char *argv[])
{

    test_high_die();
    test_low_die();
    test_median_die();
    test_random_die();

    test_ascending_die();
    test_ascending_die_too_low();
    test_ascending_die_too_high();

    test_fixed_die();
    test_fixed_die_too_low();
    test_fixed_die_too_high();

    test_mod_make();
    test_mod_make_normalizes_negative_values();

    test_dice_alloc();
    test_dice_alloc_for_zero_count();
    test_dice_alloc_with_mods();
    test_dice_alloc_with_mods_capacity();

    test_dice_alloc_parse();
    test_dice_alloc_parse_when_invalid();
    test_dice_alloc_parse_for_overflow();

    test_dice_description_alloc();

    test_dice_realloc_add_mod();
    test_dice_realloc_set_mods_capacity_larger();

    test_roll_alloc();

    test_roll_total();

    test_roll_dice();
    test_roll_dice_with_zero_count();

    test_roll();
    test_roll_with_mod();
    test_roll_with_mods();

    test_roll_parse();

    return EXIT_SUCCESS;
}
