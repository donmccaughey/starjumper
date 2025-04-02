#include "dice.h"

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MODS_SIZE(mods_capacity) \
        (sizeof(struct mod) * mods_capacity)

#define DICE_SIZE(mods_capacity) \
        (sizeof(struct dice) + MODS_SIZE(mods_capacity))

#define MODS_CAP 4

#define DICE_STORAGE DICE_SIZE(MODS_CAP)


#define DIE_ROLLS_SIZE(die_rolls_count) \
        (sizeof(int) * die_rolls_count)

#define ROLL_SIZE(die_rolls_count) \
        (sizeof(struct roll) + DIE_ROLLS_SIZE(die_rolls_count))

#define ROLL_CAP 4

#define ROLL_STORAGE ROLL_SIZE(ROLL_CAP)


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


struct dice *
dice_alloc(int count, int sides)
{
    assert(count >= 0);
    assert(sides > 1);

    return dice_alloc_init(count, sides, MODS_CAP, 0, NULL);
}


struct dice *
dice_alloc_with_mods(int count, int sides, struct mod mods[], int mods_count)
{
    assert(count >= 0);
    assert(sides > 1);
    assert(mods_count >= 0);
    assert( ! mods_count || mods);

    int mods_capacity = mods_count;
    return dice_alloc_init(count, sides, mods_capacity, mods_count, mods);
}


struct dice *
dice_alloc_with_mods_capacity(int count, int sides, int mods_capacity)
{
    assert(count >= 0);
    assert(sides > 1);
    assert(mods_capacity >= 0);

    return dice_alloc_init(count, sides, mods_capacity, 0, NULL);
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

    int mods_capacity = mods_count;
    return dice_alloc_init(count, sides, mods_capacity, mods_count, mods);
}


struct dice *
dice_alloc_init(int count, int sides, int mods_capacity, int mods_count, struct mod mods[])
{
    assert(count >= 0);
    assert(sides > 1);
    assert(mods_capacity >= 0);
    assert(mods_count >= 0);
    assert(mods_count <= mods_capacity);
    assert( ! mods_count || mods);

    struct dice *dice = malloc(DICE_SIZE(mods_capacity));
    if ( ! dice) abort();
    dice_init(dice, count, sides, mods_capacity, mods_count, mods);

    return dice;
}


void
dice_init(struct dice *dice, int count, int sides, int mods_capacity, int mods_count, struct mod mods[])
{
    assert(count >= 0);
    assert(sides > 1);
    assert(mods_capacity >= 0);
    assert(mods_count >= 0);
    assert(mods_count <= mods_capacity);
    assert( ! mods_count || mods);

    dice->count = count;
    dice->sides = sides;
    dice->mods_capacity = mods_capacity;
    dice->mods_count = mods_count;
    if (mods_count) memcpy(dice->mods, mods, MODS_SIZE(mods_count));
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

    dice = realloc(dice, DICE_SIZE(mods_capacity));
    if ( ! dice) abort();

    dice->mods_capacity = mods_capacity;
    if (dice->mods_count > mods_capacity) {
        dice->mods_count = mods_capacity;
    }

    return dice;
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


struct roll *
roll_alloc(struct dice const *dice, struct die die)
{
    assert(dice);
    assert(die.roll);

    struct roll *roll = malloc(ROLL_SIZE(dice->count));
    if ( ! roll) abort();
    roll_init(roll, dice, die);

    return roll;
}


void
roll_init(struct roll *roll, struct dice const *dice, struct die die)
{
    assert(roll);
    assert(dice);
    assert(die.roll);

    roll->dice = dice;
    roll->die_rolls_count = dice->count;
    for (int i = 0; i < dice->count; ++i) {
        roll->die_rolls[i] = roll_die(dice->sides, die);
    }
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

    int total = 0;
    if (dice->count > ROLL_CAP) {
        struct roll *roll = roll_alloc(dice, die);
        total = roll_total(roll);
        free(roll);
    } else {
        union {
            struct roll roll;
            char bytes[ROLL_STORAGE];
        } storage;
        roll_init(&storage.roll, dice, die);
        total = roll_total(&storage.roll);
    }

    return total;
}


int
roll(int count, int sides, struct die die)
{
    assert(count >= 0);
    assert(sides > 1);
    assert(die.roll);

    return roll_with_mods(count, sides, NULL, 0, die);
}


int
roll_with_mod(int count, int sides, struct mod mod, struct die die)
{
    assert(count >= 0);
    assert(sides > 1);
    assert(die.roll);

    return roll_with_mods(count, sides, &mod, 1, die);
}


int
roll_with_mods(int count, int sides, struct mod mods[], int mods_count, struct die die)
{
    assert(count >= 0);
    assert(sides > 1);
    assert( ! mods_count || mods);
    assert(die.roll);

    int total = 0;
    if (mods_count > MODS_CAP) {
        struct dice *dice = dice_alloc_with_mods(count, sides, mods, mods_count);
        total = roll_dice(dice, die);
        free(dice);
    } else {
        union {
            struct dice dice;
            char bytes[DICE_STORAGE];
        } storage;
        dice_init(&storage.dice, count, sides, MODS_CAP, mods_count, mods);
        total = roll_dice(&storage.dice, die);
    }

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
