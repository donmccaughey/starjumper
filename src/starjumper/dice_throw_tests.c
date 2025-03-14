#include <starjumper/starjumper.h>

#include <checks.h>
#include <lrnd.h>
#include <stdlib.h>


static void
test_alloc(void)
{
    struct lrnd *lrnd = lrnd_alloc_fake_fixed(3);
    struct sj_dice_throw *dice_throw = sj_dice_throw_alloc(2, 6, lrnd);

    check_int_eq(dice_throw->count, 2);
    check_int_eq(dice_throw->sides, 6);

    check_not_null(dice_throw->rolls);
    check_int_eq(dice_throw->rolls[0], 4);
    check_int_eq(dice_throw->rolls[0], 4);

    check_int_eq(dice_throw->modifiers_count, 0);

    check_int_eq(sj_dice_throw_total(dice_throw), 8);

    sj_dice_throw_free(dice_throw);
    lrnd_free(lrnd);
}


static void
test_add_modifiers(void)
{
    struct lrnd *lrnd = lrnd_alloc_fake_fixed(3);
    struct sj_dice_throw *dice_throw = sj_dice_throw_alloc(2, 6, lrnd);

    check_int_eq(dice_throw->modifiers_count, 0);
    check_int_eq(sj_dice_throw_total(dice_throw), 8);

    sj_dice_throw_add_modifier(dice_throw, 2);

    check_int_eq(dice_throw->modifiers_count, 1);
    check_int_eq(dice_throw->modifiers[0], 2);
    check_int_eq(sj_dice_throw_total(dice_throw), 10);

    sj_dice_throw_free(dice_throw);
    lrnd_free(lrnd);
}


static void
test_string_alloc(void)
{
    struct lrnd *lrnd = lrnd_alloc_fake_fixed(3);
    struct sj_dice_throw *dice_throw = sj_dice_throw_alloc(2, 6, lrnd);
    char *s = sj_string_alloc_from_dice_throw(dice_throw);

    check_str_eq(s, "2D6 (4, 4) ");

    free(s);
    sj_dice_throw_free(dice_throw);
    lrnd_free(lrnd);
}


int
main(int argc, char *argv[])
{
    test_alloc();
    test_add_modifiers();
    test_string_alloc();
    return EXIT_SUCCESS;
}
