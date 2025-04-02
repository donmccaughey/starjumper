#include "world.h"

#include <assert.h>
#include <dice.h>
#include <string.h>
#include <xmalloc.h>
#include <xstring.h>
#include <xstdio.h>

#include "string_array.h"
#include "string_join.h"
#include "trade_classification.h"


static char *
alloc_trade_classification_abbreviation(void const *item);

static char *
alloc_trade_classification_name(void const *item);

static char *
alloc_trade_classification_short_name(void const *item);

static char
base_code(struct sj_world const *world);

static char
hex_digit(int value);


static char *
alloc_trade_classification_abbreviation(void const *item)
{
    struct sj_trade_classification const *const *trade_classification = item;
    return xstrdup((*trade_classification)->abbreviation);
}


static char *
alloc_trade_classification_name(void const *item)
{
    struct sj_trade_classification const *const *trade_classification = item;
    return xstrdup((*trade_classification)->name);
}


static char *
alloc_trade_classification_short_name(void const *item)
{
    struct sj_trade_classification const *const *trade_classification = item;
    return xstrdup((*trade_classification)->short_name);
}


static char
base_code(struct sj_world const *world)
{
    if (world->naval_base) {
        return world->scout_base ? 'A' : 'N'; // base code 'A' from Supplement 10: The Solomani Rim
    } else if (world->scout_base) {
        return 'S';
    } else {
        return ' ';
    }
}


char *
sj_string_from_world(struct sj_world const *world)
{

    if ( ! world) {
        return xstrdup("Name               Statistics       Remarks");
    }

    int const max_name_length = 18;
    int const max_classifications_length = 42;

    char *hex_coordinate = sj_string_alloc_from_hex_coordinate(world->hex_coordinate);

    char const separator[] = ". ";

    struct sj_string_array *classification_names = sj_string_array_alloc_collect_strings(
            world->trade_classifications,
            world->trade_classifications_count,
            sizeof world->trade_classifications[0],
            alloc_trade_classification_name
    );
    char *classifications = sj_string_alloc_join_string_array_with_suffix(classification_names,
                                                                          separator);
    sj_string_array_free(classification_names);

    if (strlen(classifications) > max_classifications_length) {
        free(classifications);
        struct sj_string_array *classification_short_names = sj_string_array_alloc_collect_strings(
                world->trade_classifications,
                world->trade_classifications_count,
                sizeof world->trade_classifications[0],
                alloc_trade_classification_short_name
        );
        classifications = sj_string_alloc_join_string_array_with_suffix(classification_short_names,
                                                                        separator);
        sj_string_array_free(classification_short_names);

        if (strlen(classifications) > max_classifications_length) {
            free(classifications);
            struct sj_string_array *classification_abbreviations = sj_string_array_alloc_collect_strings(
                    world->trade_classifications,
                    world->trade_classifications_count,
                    sizeof world->trade_classifications[0],
                    alloc_trade_classification_abbreviation
            );
            classifications = sj_string_alloc_join_string_array_with_suffix(classification_abbreviations,
                                                                            separator);
            sj_string_array_free(classification_abbreviations);
        }
    }

    char *string;
    xasprintf(&string, "%*s %4s %c%c%c%c%c%c%c-%c %c %*s%c",
              -max_name_length, world->name, hex_coordinate,
              world->starport,
              hex_digit(world->size), hex_digit(world->atmosphere),
              hex_digit(world->hydrographics), hex_digit(world->population),
              hex_digit(world->government), hex_digit(world->law_level),
              hex_digit(world->tech_level), base_code(world),
              -max_classifications_length, classifications,
              (world->gas_giant ? 'G' : ' ')
    );

    free(classifications);
    free(hex_coordinate);
    return string;
}


static char
hex_digit(int value)
{
    assert(value >= 0);
    assert(value <= 34);

    if (value < 10) {
        return (char) ('0' + value);
    } else {
        return (char) ('A' + value - 10);
    }
}


static char
generate_starport(struct die die)
{
    static char *starport_table = "??AAABBCCDEEX";
    int i = roll(2, 6, die);
    return starport_table[i];
}


static bool
generate_naval_base(char starport, struct die die)
{
    static bool naval_base_table[] = {
            false,
            false,
            false, // 2
            false, // 3
            false, // 4
            false, // 5
            false, // 6
            false, // 7
            true,  // 8
            true,  // 9
            true,  // 10
            true,  // 11
            true,  // 12
    };

    if ('A' == starport || 'B' == starport) {
        int i = roll(2, 6, die);
        return naval_base_table[i];
    } else {
        return false;
    }
}


static bool
generate_scout_base(char starport, struct die die)
{
    static bool scout_base_table[] = {
            false,
            false,
            false, // 2
            false, // 3
            false, // 4
            false, // 5
            false, // 6
            true,  // 7
            true,  // 8
            true,  // 9
            true,  // 10
            true,  // 11
            true,  // 12
    };

    int modifier = 0;
    switch (starport) {
        case 'A':
            modifier = -3; break;
        case 'B':
            modifier = -2; break;
        case 'C':
            modifier = -1; break;
        case 'D':
            modifier = 0; break;
        case 'E':
            return false;
        case 'X':
            return false;
        default:
            abort();
    }

    int i = roll_with_mod(2, 6, mod_make('+', modifier), die);
    if (i < 2) i = 2;
    return scout_base_table[i];
}


static bool
generate_gas_giant(struct die die)
{
    static bool gas_giant_table[] = {
            false,
            false,
            true,  // 2
            true,  // 3
            true,  // 4
            true,  // 5
            true,  // 6
            true,  // 7
            true,  // 8
            true,  // 9
            false, // 10
            false, // 11
            false, // 12
    };

    int i = roll(2, 6, die);
    return gas_giant_table[i];
}


static int
generate_size(struct die die)
{
    return roll_with_mod(2, 6, mod_make('-', 2), die);
}


static int
generate_atmosphere(int size, struct die die)
{
    if (0 == size) return 0;

    struct mod mods[] = {
            mod_make('-', 7),
            mod_make('+', size),
    };
    int mods_count = sizeof(mods) / sizeof(mods[0]);

    int atmosphere = roll_with_mods(2, 6, mods, mods_count, die);
    return atmosphere < 0 ? 0 : atmosphere;
}


static int
generate_hydrographics(int size, int atmosphere, struct die die)
{
    if (0 == size) return 0;

    struct dice *dice = dice_alloc_with_mods_capacity(2, 6, 3);
    dice = dice_realloc_add_mod(dice, mod_make('-', 7));
    dice = dice_realloc_add_mod(dice, mod_make('+', atmosphere));
    if (0 == atmosphere || 1 == atmosphere || atmosphere >= 10) {
        dice = dice_realloc_add_mod(dice, mod_make('-', 4));
    }

    int hydrographics = roll_dice(dice, die);
    free(dice);

    if (hydrographics < 0) hydrographics = 0;
    if (hydrographics > 10) hydrographics = 10;
    return hydrographics;
}


static int
generate_population(struct die die)
{
    return roll_with_mod(2, 6, mod_make('-', 2), die);
}


static int
generate_government(int population, struct die die)
{
    if (0 == population) return 0;

    struct mod mods[] = {
            mod_make('-', 7),
            mod_make('+', population),
    };
    int mods_count = sizeof(mods) / sizeof(mods[0]);

    int government = roll_with_mods(2, 6, mods, mods_count, die);
    return government < 0 ? 0 : government;
}


static int
generate_law_level(int population, int government, struct die die)
{
    if (0 == population) return 0;

    struct mod mods[] = {
            mod_make('-', 7),
            mod_make('+', government),
    };
    int mods_count = sizeof(mods) / sizeof(mods[0]);

    int law_level = roll_with_mods(2, 6, mods, mods_count, die);
    if (law_level < 0) law_level = 0;
    return law_level;
}


static int
generate_tech_level(struct sj_world const *world, struct die die)
{
    if (0 == world->population) return 0;

    struct dice *dice = dice_alloc_with_mods_capacity(1, 6, 6);

    if ('A' == world->starport) {
        dice = dice_realloc_add_mod(dice, mod_make('+', 6));
    } else if ('B' == world->starport) {
        dice = dice_realloc_add_mod(dice, mod_make('+', 4));
    } else if ('C' == world->starport) {
        dice = dice_realloc_add_mod(dice, mod_make('+', 2));
    } else if ('X' == world->starport) {
        dice = dice_realloc_add_mod(dice, mod_make('-', 4));
    }

    static int size_mods[] = {
            +2, // 0
            +2, // 1
            +1, // 2
            +1, // 3
            +1, // 4
            0, // 5
            0, // 6
            0, // 7
            0, // 8
            0, // 9
            0, // 10
    };
    assert(world->size >= 0);
    assert(world->size <= 10);
    dice = dice_realloc_add_mod(dice, mod_make('+', size_mods[world->size]));

    static int atmosphere_mods[] = {
            +1, // 0
            +1, // 1
            +1, // 2
            +1, // 3
            0, // 4
            0, // 5
            0, // 6
            0, // 7
            0, // 8
            0, // 9
            +1, // 10
            +1, // 11
            +1, // 12
            +1, // 13
            +1, // 14
            0, // 15
    };
    assert(world->atmosphere >= 0);
    assert(world->atmosphere <= 15);
    dice = dice_realloc_add_mod(dice, mod_make('+', atmosphere_mods[world->atmosphere]));

    static int hydrographics_mods[] = {
            0, // 0
            0, // 1
            0, // 2
            0, // 3
            0, // 4
            0, // 5
            0, // 6
            0, // 7
            0, // 8
            +1, // 9
            +2, // 10
    };
    assert(world->hydrographics >= 0);
    assert(world->hydrographics <= 10);
    dice = dice_realloc_add_mod(dice, mod_make('+', hydrographics_mods[world->hydrographics]));

    static int population_mods[] = {
        0, // 0
        +1, // 1
        +1, // 2
        +1, // 3
        +1, // 4
        +1, // 5
        0, // 6
        0, // 7
        0, // 8
        +2, // 9
        +4, // 10
    };
    assert(world->population >= 0);
    assert(world->population <= 10);
    dice = dice_realloc_add_mod(dice, mod_make('+', population_mods[world->population]));

    static int government_mods[] = {
            +1, // 0
            0, // 1
            0, // 2
            0, // 3
            0, // 4
            +1, // 5
            0, // 6
            0, // 7
            0, // 8
            0, // 9
            0, // 10
            0, // 11
            0, // 12
            -2, // 13
            0, // 14
            0, // 15
    };
    assert(world->government >= 0);
    assert(world->government <= 15);
    dice = dice_realloc_add_mod(dice, mod_make('+', government_mods[world->government]));

    int tech_level = roll_dice(dice, die);
    free(dice);
    if (tech_level < 0) tech_level = 0;
    return tech_level;
}


struct sj_world *
sj_world_alloc(char const *name,
               struct sj_hex_coordinate const hex_coordinate,
               struct die die)
{
    struct sj_world *world = xmalloc(sizeof(struct sj_world));

    world->name = xstrdup(name);
    world->hex_coordinate = hex_coordinate;

    // TODO: adjust starport and bases if 0 == population
    world->starport = generate_starport(die);
    world->naval_base = generate_naval_base(world->starport, die);
    world->scout_base = generate_scout_base(world->starport, die);
    world->gas_giant = generate_gas_giant(die);

    world->size = generate_size(die);
    world->atmosphere = generate_atmosphere(world->size, die);
    world->hydrographics = generate_hydrographics(world->size, world->atmosphere, die);
    world->population = generate_population(die);
    world->government = generate_government(world->population, die);
    world->law_level = generate_law_level(world->population, world->government, die);

    world->tech_level = generate_tech_level(world, die);
    world->trade_classifications = sj_world_alloc_trade_classifications(world, &world->trade_classifications_count);

    return world;
}


void
sj_world_free(struct sj_world *world)
{
    free(world->name);
    free(world->trade_classifications);
    free(world);
}
