#include "sj_world.h"

#include <assert.h>
#include <string.h>

#include "sj_dice_throw.h"
#include "sj_die_modifier.h"
#include "sj_die_modifier_array.h"
#include "sj_hex_coordinate.h"
#include "sj_memory.h"
#include "sj_string_array.h"
#include "sj_string.h"
#include "sj_trade_classification.h"


struct _sj_world
{
  SF_OBJECT_FIELDS;
  sf_string_t name;
  struct sj_hex_coordinate hex_coordinate;
  char starport;
  bool naval_base;
  bool scout_base;
  bool gas_giant;
  int size;
  int atmosphere;
  int hydrographics;
  int population;
  int government;
  int law_level;
  int tech_level;
  struct sj_trade_classification const **trade_classifications;
  int trade_classifications_count;
};


static char *
alloc_trade_classification_abbreviation(void const *item);

static char *
alloc_trade_classification_name(void const *item);

static char *
alloc_trade_classification_short_name(void const *item);

static char
base_code(sj_world_t world);

static void
dealloc(sf_any_t self);

static sf_string_t 
string_from(sf_any_t self);

static char
hex_digit(int value);


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

static int tech_level_atmosphere_table[] = {
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
};

static int tech_level_government_table[] = {
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

static int tech_level_hydrographics_table[] = {
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

static int tech_level_population_table[] = {
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

static int tech_level_size_table[] = {
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

static char *starport_table = "??AAABBCCDEEX";

sf_type_t sj_world_type;


void
_sj_world_init(void)
{
  sj_world_type = sf_type("sj_world_t", dealloc, string_from, NULL, NULL, NULL, NULL);
}


static char *
alloc_trade_classification_abbreviation(void const *item)
{
  struct sj_trade_classification const *const *trade_classification = item;
  return sj_strdup((*trade_classification)->abbreviation);
}


static char *
alloc_trade_classification_name(void const *item)
{
  struct sj_trade_classification const *const *trade_classification = item;
  return sj_strdup((*trade_classification)->name);
}


static char *
alloc_trade_classification_short_name(void const *item)
{
  struct sj_trade_classification const *const *trade_classification = item;
  return sj_strdup((*trade_classification)->short_name);
}


static char
base_code(sj_world_t world)
{
  if (world->naval_base) {
    return world->scout_base ? 'A' : 'N'; // base code 'A' from Supplement 10: The Solomani Rim
  } else if (world->scout_base) {
    return 'S';
  } else {
    return ' ';
  }
}


static void
dealloc(sf_any_t self)
{
  sj_world_t world = self;
  
  sf_release(world->name);
  sj_free(world->trade_classifications);
}


static sf_string_t 
string_from(sf_any_t self)
{
  sj_world_t world = self;
  
  char *hex_coordinate = sj_string_alloc_from_hex_coordinate(world->hex_coordinate);
  
  int const max_name_length = 18;
  int const max_classifications_length = 42;
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
    sj_free(classifications);
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
      sj_free(classifications);
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
  
  sf_string_t string = sf_string_from_format("%*s %4s %c%c%c%c%c%c%c-%c %c %*s%c",
      -max_name_length, sf_string_chars(world->name), hex_coordinate,
      world->starport,
      hex_digit(world->size), hex_digit(world->atmosphere),
      hex_digit(world->hydrographics), hex_digit(world->population),
      hex_digit(world->government), hex_digit(world->law_level),
      hex_digit(world->tech_level), base_code(world),
      -max_classifications_length, classifications,
      (world->gas_giant ? 'G' : ' ')
  );
  
  sj_free(classifications);
  sj_free(hex_coordinate);
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


sj_world_t
sj_world(sf_string_t name,
         struct sj_hex_coordinate const hex_coordinate,
         struct sj_random *random)
{
  struct _sj_world *world = sf_object_calloc(sizeof(struct _sj_world), sj_world_type);
  if ( ! world) return NULL;
  
  world->name = sf_retain(name);
  world->hex_coordinate = hex_coordinate;
  
  sj_dice_throw_t dice_throw = NULL;
  
  dice_throw = sj_dice_throw(2, 6, sj_die_modifier_array_alloc_empty(), random);
  int starport_index = sj_dice_throw_total(dice_throw);
  world->starport = starport_table[starport_index];
  
  if ('A' == world->starport || 'B' == world->starport) {
    dice_throw = sj_dice_throw(2, 6, sj_die_modifier_array_alloc_empty(), random);
    int naval_base_index = sj_dice_throw_total(dice_throw);
    world->naval_base = naval_base_table[naval_base_index];
  }
  
  if ('E' != world->starport && 'X' != world->starport) {
    struct sj_die_modifier_array *die_modifiers = sj_die_modifier_array_alloc_empty();
    if ('C' == world->starport) {
      sj_die_modifier_array_add(die_modifiers, (struct sj_die_modifier) {.value=-1});
    } else if ('B' == world->starport) {
      sj_die_modifier_array_add(die_modifiers, (struct sj_die_modifier) {.value=-2});
    } else if ('A' == world->starport) {
      sj_die_modifier_array_add(die_modifiers, (struct sj_die_modifier) {.value=-3});
    }
    dice_throw = sj_dice_throw(2, 6, die_modifiers, random);
    int scout_base_index = sj_dice_throw_total(dice_throw);
    world->scout_base = scout_base_table[scout_base_index];
  }
  
  dice_throw = sj_dice_throw(2, 6, sj_die_modifier_array_alloc_empty(), random);
  int gas_giant_index = sj_dice_throw_total(dice_throw);
  world->gas_giant = gas_giant_table[gas_giant_index];
  
  struct sj_die_modifier_array *die_modifiers = sj_die_modifier_array_alloc(
      (struct sj_die_modifier[]) {{.value=-2}}, 1
  );
  dice_throw = sj_dice_throw(2, 6, die_modifiers, random);
  world->size = sj_dice_throw_total(dice_throw);
  
  if (0 == world->size) {
    world->atmosphere = 0;
  } else {
    struct sj_die_modifier_array *die_modifiers = sj_die_modifier_array_alloc(
        (struct sj_die_modifier[]) {{.value=-7}, {.value=world->size}}, 2
    );
    dice_throw = sj_dice_throw(2, 6, die_modifiers, random);
    world->atmosphere = sj_dice_throw_total(dice_throw);
    if (world->atmosphere < 0) world->atmosphere = 0;
  }
  
  if (0 == world->size) {
    world->hydrographics = 0;
  } else {
    struct sj_die_modifier_array *die_modifiers = sj_die_modifier_array_alloc(
        (struct sj_die_modifier[]) {{.value=-7}, {.value=world->atmosphere}}, 2
    );
    if (0 == world->atmosphere || 1 == world->atmosphere || world->atmosphere >= 10) {
      sj_die_modifier_array_add(die_modifiers, (struct sj_die_modifier) {.value=-4});
    }
    dice_throw = sj_dice_throw(2, 6, die_modifiers, random);
    world->hydrographics = sj_dice_throw_total(dice_throw);
    if (world->hydrographics < 0) world->hydrographics = 0;
    if (world->hydrographics > 10) world->hydrographics = 10;
  }
  
  die_modifiers = sj_die_modifier_array_alloc(
      (struct sj_die_modifier[]) {{.value=-2}}, 1
  );
  dice_throw = sj_dice_throw(2, 6, die_modifiers, random);
  world->population = sj_dice_throw_total(dice_throw);
  
  if (0 == world->population) {
    world->government = 0;
  } else {
    struct sj_die_modifier_array *die_modifiers = sj_die_modifier_array_alloc(
        (struct sj_die_modifier[]) {{.value=-7}, {.value=world->population}}, 2
    );
    dice_throw = sj_dice_throw(2, 6, die_modifiers, random);
    world->government = sj_dice_throw_total(dice_throw);
    if (world->government < 0) world->government = 0;
  }
  
  if (0 == world->population) {
    world->law_level = 0;
  } else {
    struct sj_die_modifier_array *die_modifiers = sj_die_modifier_array_alloc(
        (struct sj_die_modifier[]) {{.value=-7}, {.value=world->government}}, 2
    );
    dice_throw = sj_dice_throw(2, 6, die_modifiers, random);
    world->law_level = sj_dice_throw_total(dice_throw);
    if (world->law_level < 0) world->law_level = 0;
  }
  
  // TODO: adjust starport if 0 == population
  
  if (world->population) {
    struct sj_die_modifier_array *die_modifiers = sj_die_modifier_array_alloc_empty();
    
    if ('A' == world->starport) {
      sj_die_modifier_array_add(die_modifiers, (struct sj_die_modifier) {.value=+6});
    } else if ('B' == world->starport) {
      sj_die_modifier_array_add(die_modifiers, (struct sj_die_modifier) {.value=+4});
    } else if ('C' == world->starport) {
      sj_die_modifier_array_add(die_modifiers, (struct sj_die_modifier) {.value=+2});
    } else if ('X' == world->starport) {
      sj_die_modifier_array_add(die_modifiers, (struct sj_die_modifier) {.value=-4});
    }
    
    sj_die_modifier_array_add(die_modifiers, (struct sj_die_modifier) {.value=tech_level_size_table[world->size]});
    sj_die_modifier_array_add(die_modifiers, (struct sj_die_modifier) {.value=tech_level_atmosphere_table[world->atmosphere]});
    sj_die_modifier_array_add(die_modifiers, (struct sj_die_modifier) {.value=tech_level_hydrographics_table[world->hydrographics]});
    sj_die_modifier_array_add(die_modifiers, (struct sj_die_modifier) {.value=tech_level_population_table[world->population]});
    sj_die_modifier_array_add(die_modifiers, (struct sj_die_modifier) {.value=tech_level_government_table[world->government]});
    
    dice_throw = sj_dice_throw(1, 6, die_modifiers, random);
    world->tech_level = sj_dice_throw_total(dice_throw);
    if (world->tech_level < 0) world->tech_level = 0;
  }
  
  world->trade_classifications = sj_world_alloc_trade_classifications(world, &world->trade_classifications_count);
  
  return sf_move_to_temp_pool(world);
}


int
sj_world_atmosphere(sj_world_t world)
{
  return world ? world->atmosphere : 0;
}


int
sj_world_government(sj_world_t world)
{
  return world ? world->government : 0;
}


struct sj_hex_coordinate
sj_world_hex_coordinate(sj_world_t world)
{
  return world ? world->hex_coordinate : (struct sj_hex_coordinate) { .horizontal=0, .vertical=0, };
}


int
sj_world_hydrographics(sj_world_t world)
{
  return world ? world->hydrographics : 0;
}


int
sj_world_law_level(sj_world_t world)
{
  return world ? world->law_level : 0;
}


sf_string_t
sj_world_name(sj_world_t world)
{
  return world ? world->name : NULL;
}


int
sj_world_population(sj_world_t world)
{
  return world ? world->population : 0;
}


int
sj_world_size(sj_world_t world)
{
  return world ? world->size : 0;
}


char
sj_world_starport(sj_world_t world)
{
  return world ? world->starport : 'X';
}