#include <starjumper/starjumper.h>

#include <checks.h>
#include <rnd.h>
#include <stdlib.h>


static void
test_dice_throw(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(3);

    int total = sj_dice_throw(2, 6, NULL, 0, rnd);
    check_int_eq(total, 8);

    total = sj_dice_throw(2, 6, (int[]) { 2 }, 1, rnd);
    check_int_eq(total, 10);

    total = sj_dice_throw(2, 6, (int[]) { 2, 3 }, 2, rnd);
    check_int_eq(total, 13);

    rnd_free(rnd);
}


static void
test_alloc(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(3);
    struct sj_dice_throw *dice_throw = sj_dice_throw_alloc(2, 6, rnd);

    check_int_eq(dice_throw->count, 2);
    check_int_eq(dice_throw->sides, 6);

    check_not_null(dice_throw->rolls);
    check_int_eq(dice_throw->rolls[0], 4);
    check_int_eq(dice_throw->rolls[0], 4);

    check_int_eq(dice_throw->modifiers_count, 0);

    check_int_eq(sj_dice_throw_total(dice_throw), 8);

    sj_dice_throw_free(dice_throw);
    rnd_free(rnd);
}


static void
test_add_modifiers(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(3);
    struct sj_dice_throw *dice_throw = sj_dice_throw_alloc(2, 6, rnd);

    check_int_eq(dice_throw->modifiers_count, 0);
    check_int_eq(sj_dice_throw_total(dice_throw), 8);

    sj_dice_throw_add_modifier(dice_throw, 2);

    check_int_eq(dice_throw->modifiers_count, 1);
    check_int_eq(dice_throw->modifiers[0], 2);
    check_int_eq(sj_dice_throw_total(dice_throw), 10);

    sj_dice_throw_free(dice_throw);
    rnd_free(rnd);
}


static void
test_string_alloc(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(3);
    struct sj_dice_throw *dice_throw = sj_dice_throw_alloc(2, 6, rnd);
    char *s = sj_string_alloc_from_dice_throw(dice_throw);

    check_str_eq(s, "2D6 (4, 4) ");

    free(s);
    sj_dice_throw_free(dice_throw);
    rnd_free(rnd);
}


int
main(int argc, char *argv[])
{
    test_dice_throw();
    test_alloc();
    test_add_modifiers();
    test_string_alloc();
    return EXIT_SUCCESS;
}
