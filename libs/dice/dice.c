#include "dice.h"

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int
roll_ascending(int sides, void *state)
{
    assert(sides > 1);
    assert(state);

    int *ascending_value = state;
    int next_value = *ascending_value;
    ++*ascending_value;
    if (next_value < 1) return 1;
    return (next_value - 1) % sides + 1;
}


static int
roll_fixed(int sides, void *state)
{
    assert(sides > 1);
    assert(state);

    int *fixed_value = state;
    if (*fixed_value < 1) return 1;
    if (*fixed_value > sides) return sides;
    return *fixed_value;
}


static int
roll_high(int sides, void *state)
{
    assert(sides > 1);

    return sides;
}


static int
roll_low(int sides, void *state)
{
    assert(sides > 1);

    return 1;
}


static int
roll_median(int sides, void *state)
{
    assert(sides > 1);

    return sides / 2 + 1;
}


static int
roll_random(int sides, void *state)
{
    assert(sides > 1);

    return (int)arc4random_uniform(sides) + 1;
}


struct die random_die = {
    .roll=roll_random,
    .state=NULL,
};

struct die high_die = {
    .roll=roll_high,
    .state=NULL,
};


struct die low_die = {
    .roll=roll_low,
    .state=NULL,
};


struct die median_die = {
    .roll=roll_median,
    .state=NULL,
};


struct die
die_make_ascending(int *initial_value)
{
    assert(initial_value);

    return (struct die){
        .roll=roll_ascending,
        .state=initial_value,
    };
}


struct die
die_make_fixed(int *fixed_value)
{
    assert(fixed_value);

    return (struct die){
        .roll=roll_fixed,
        .state=fixed_value,
    };
}


static void
normalize_negative_value(char *op, int *value)
{
    if (*value < 0) {
        if ('+' == *op) {
            *op = '-';
            *value = -*value;
        } else if ('-' == *op) {
            *op = '+';
            *value = -*value;
        }
    }
}


struct mod
mod_make(char op, int value)
{
    assert('+' == op || '-' == op || 'x' == op);

    normalize_negative_value(&op, &value);
    return (struct mod){
        .op=op,
        .value=value,
    };
}


static size_t
dice_size(int mods_capacity)
{
    size_t mods_size = sizeof(struct mod) * mods_capacity;
    return sizeof(struct dice) + mods_size;
}


struct dice *
dice_alloc(int count, int sides)
{
    assert(count >= 0);
    assert(sides > 1);

    return dice_alloc_with_mods_capacity(count, sides, 4);
}


struct dice *
dice_alloc_with_mods(int count, int sides, struct mod mods[], int mods_count)
{
    assert(count > 0);
    assert(sides > 1);
    assert(mods_count >= 0);
    assert( ! mods_count || mods);

    int mods_capacity = mods_count;

    struct dice *dice = dice_alloc_with_mods_capacity(count, sides, mods_capacity);

    dice->count = count;
    dice->sides = sides;
    dice->mods_capacity = mods_capacity;
    dice->mods_count = mods_count;
    if (mods_count) {
        size_t mods_size = sizeof(struct mod) * mods_count;
        memcpy(dice->mods, mods, mods_size);
    }

    return dice;
}


struct dice *
dice_alloc_with_mods_capacity(int count, int sides, int mods_capacity)
{
    assert(count >= 0);
    assert(sides > 1);
    assert(mods_capacity >= 0);

    struct dice *dice = malloc(dice_size(mods_capacity));
    if ( ! dice) abort();

    dice->count = count;
    dice->sides = sides;
    dice->mods_capacity = mods_capacity;
    dice->mods_count = 0;

    return dice;
}


static int
digit_value(char ch)
{
    return ch - '0';
}


static int
scan_int(char const **ch)
{
    int value = 0;
    while (isdigit(**ch)) {
        if (value > INT_MAX / 10) return -1;
        value *= 10;

        int digit = digit_value(*(*ch)++);
        if (value > INT_MAX - digit) return -1;
        value += digit;
    }
    return value;
}


struct dice *
dice_alloc_parse(char const *dice_expression)
{
    if (! dice_expression || ! *dice_expression) return NULL;

    char const *ch = dice_expression;

    int count = isdigit(*ch) ? scan_int(&ch) : 1;
    if (count < 1) return NULL;

    if ('d' != tolower(*ch++)) return NULL;

    int sides = isdigit(*ch) ? scan_int(&ch) : 6;
    if (sides < 2) return NULL;

    char const *mods_string = ch;

    int mods_count = 0;
    while (*ch) {
        if ('+' != *ch && '-' != *ch && 'x' != *ch) return NULL;
        ++ch;
        if ( ! isdigit(*ch)) return NULL;
        while (isdigit(*ch)) ++ch;
        ++mods_count;
    }

    ch = mods_string;
    struct mod mods[mods_count];

    int i = 0;
    while (*ch) {
        char op = *ch++;
        int value = scan_int(&ch);
        if (value < 0) return NULL;
        mods[i++] = mod_make(op, value);
    }

    return dice_alloc_with_mods(count, sides, mods, mods_count);
}


static size_t
digit_count(int n)
{
    assert(n >= 0);

    size_t count = 1;
    while (n >= 10) {
        n /= 10;
        ++count;
    }
    return count;
}


char *
dice_description_alloc(struct dice const *dice)
{
    assert(dice);

    size_t count = 0;

    count += digit_count(dice->count);
    count += 1;  // 'd'
    count += digit_count(dice->sides);

    for (int i = 0; i < dice->mods_count; ++i) {
        count += 1;  // op
        count += digit_count(dice->mods[i].value);
    }

    count += 1;  // null terminator

    char *description = calloc(count, sizeof(char));
    if ( ! description) abort();

    char *s = description;
    int n = snprintf(s, count, "%id%i", dice->count, dice->sides);
    assert(n > 0);
    count -= n;
    assert(count > 0);
    s += n;

    for (int i = 0; i < dice->mods_count; ++i) {
        n = snprintf(s, count, "%c%i", dice->mods[i].op, dice->mods[i].value);
        assert(n > 0);
        count -= n;
        assert(count > 0);
        s += n;
    }

    return description;
}


struct dice *
dice_realloc_add_mod(struct dice *dice, struct mod mod)
{
    assert(dice);
    assert('+' == mod.op || '-' == mod.op || 'x' == mod.op);

    if (dice->mods_count >= dice->mods_capacity) {
        int mods_capacity = (dice->mods_capacity < 4)
                ? 4
                : dice->mods_capacity / 2 * 3;
        dice = dice_realloc_set_mods_capacity(dice, mods_capacity);
    }
    int i = dice->mods_count++;
    dice->mods[i] = mod;
    return dice;
}


struct dice *
dice_realloc_set_mods_capacity(struct dice *dice, int mods_capacity)
{
    assert(dice);
    assert(mods_capacity >= 0);

    dice = realloc(dice, dice_size(mods_capacity));
    if ( ! dice) abort();

    dice->mods_capacity = mods_capacity;
    if (dice->mods_count > mods_capacity) {
        dice->mods_count = mods_capacity;
    }

    return dice;
}


static size_t
roll_size(int die_rolls_count)
{
    size_t die_rolls_size = sizeof(int) * die_rolls_count;
    return sizeof(struct roll) + die_rolls_size;
}


static void
make_die_rolls(struct dice const *dice, int die_rolls[], struct die die)
{
    for (int i = 0; i < dice->count; ++i) {
        die_rolls[i] = die.roll(dice->sides, die.state);
    }
}


struct roll *
roll_alloc(struct dice const *dice, struct die die)
{
    assert(dice);
    assert(die.roll);

    int die_rolls[dice->count];
    make_die_rolls(dice, die_rolls, die);

    return roll_alloc_with_die_rolls(dice, die_rolls, dice->count);
}


struct roll *
roll_alloc_with_die_rolls(struct dice const *dice, int die_rolls[], int die_rolls_count)
{
    assert(dice);
    assert(die_rolls_count >= 0);
    assert( ! die_rolls_count || die_rolls);

    struct roll *roll = malloc(roll_size(die_rolls_count));
    if ( ! roll) abort();

    roll->dice = dice;
    roll->die_rolls_count = die_rolls_count;
    if (die_rolls_count) {
        size_t die_rolls_size = sizeof(int) * die_rolls_count;
        memcpy(roll->die_rolls, die_rolls, die_rolls_size);
    }

    return roll;
}


int
roll_total(struct roll const *roll)
{
    assert(roll);
    assert(roll->dice);

    int total = 0;

    for (int i = 0; i < roll->die_rolls_count; ++i) {
        total += roll->die_rolls[i];
    }

    for (int i = 0; i < roll->dice->mods_count; ++i) {
        struct mod mod = roll->dice->mods[i];
        switch (mod.op) {
            case '+':
                total += mod.value;
                break;
            case '-':
                total -= mod.value;
                break;
            case 'x':
                total *= mod.value;
                break;
            default:
                assert('+' == mod.op || '-' == mod.op || 'x' == mod.op);
        }
    }

    return total;
}


int
roll_die(int sides, struct die die)
{
    assert(sides > 1);
    assert(die.roll);

    return die.roll(sides, die.state);
}


int
roll_dice(struct dice const *dice, struct die die)
{
    assert(dice);
    assert(die.roll);

    struct roll *roll = roll_alloc(dice, die);
    int total = roll_total(roll);
    free(roll);

    return total;
}


int
roll(int count, int sides, struct die die)
{
    assert(count >= 0);
    assert(sides > 1);

    return roll_with_mods(count, sides, NULL, 0, die);
}


int
roll_with_mod(int count, int sides, struct mod mod, struct die die)
{
    assert(count >= 0);
    assert(sides > 1);

    return roll_with_mods(count, sides, &mod, 1, die);
}


int
roll_with_mods(int count, int sides, struct mod mods[], int mods_count, struct die die)
{
    assert(count >= 0);
    assert(sides > 1);

    struct dice *dice = dice_alloc_with_mods(count, sides, mods, mods_count);

    int total = roll_dice(dice, die);
    free(dice);

    return total;
}


int
roll_parse(char const *dice_expression, struct die die)
{
    assert(dice_expression);
    assert(*dice_expression);
    assert(die.roll);

    struct dice *dice = dice_alloc_parse(dice_expression);
    assert(dice);

    int total = roll_dice(dice, die);
    free(dice);

    return total;
}
