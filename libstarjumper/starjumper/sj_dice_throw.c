#include "sj_dice_throw.h"

#include <assert.h>
#include "sj_die_modifier.h"
#include "sj_die_modifier_array.h"
#include "sj_memory.h"


static void
dealloc(sf_any_t self);

static sf_any_t
get_die_roll_value_string(sf_any_t any, void *context);

static sf_string_t 
string_from(sf_any_t self);


struct _sj_dice_throw
{
  SF_OBJECT_FIELDS;
  int count;
  int sides;
  struct sj_die_modifier_array *die_modifier_array;
  sf_list_t die_rolls;
};


sf_type_t sj_dice_throw_type;


void
_sj_dice_throw_init(void)
{
  sj_dice_throw_type = sf_type("sj_dice_throw_t", dealloc, string_from, NULL, NULL, NULL, NULL);
}


static void
dealloc(sf_any_t self)
{
  sj_dice_throw_t dice_throw = self;
  sj_die_modifier_array_free(dice_throw->die_modifier_array);
  sf_release(dice_throw->die_rolls);
}


static sf_any_t
get_die_roll_value_string(sf_any_t any, void *context)
{
  int value = sf_random_value(any);
  return sf_string_from_format("%i", value);
}


static sf_string_t 
string_from(sf_any_t self)
{
  sj_dice_throw_t dice_throw = self;
  
  sf_string_t dice = sf_string_from_format("%iD%i", dice_throw->count, dice_throw->sides);
  sf_list_t parts = sf_list(dice, NULL);
  
  if (sf_count(dice_throw->die_rolls)) {
    sf_list_t die_rolls = sf_collect(dice_throw->die_rolls, get_die_roll_value_string, NULL);
    sf_string_t die_rolls_string = sf_string_from_separator_and_collection(sf_string(", "), die_rolls);
    die_rolls_string = sf_string_from_format("(%s)", sf_string_chars(die_rolls_string));
    parts = sf_list(die_rolls_string, parts);
  }
  
  for (int i = 0; i < dice_throw->die_modifier_array->count; ++i) {
    if (i) parts = sf_list(sf_string(", "), parts);
    char *die_modifier_string = sj_string_alloc_from_die_modifier(dice_throw->die_modifier_array->elements[i]);
    parts = sf_list(sf_string(die_modifier_string), parts);
    sj_free(die_modifier_string);
  }
  
  parts = sf_list_reversed(parts);
  return sf_string_from_separator_and_collection(sf_string(" "), parts);
}


sj_dice_throw_t
sj_dice_throw(int count,
              int sides,
              struct sj_die_modifier_array *die_modifier_array,
              sf_random_t random_in,
              sf_random_t *random_out)
{
  assert(count > 0);
  assert(sides > 1);
  assert(random_out);
  
  struct _sj_dice_throw *dice_throw = sf_object_calloc(sizeof(struct _sj_dice_throw), sj_dice_throw_type);
  if ( ! dice_throw) return NULL;
  
  dice_throw->count = count;
  dice_throw->sides = sides;
  dice_throw->die_modifier_array = die_modifier_array;
  dice_throw->die_rolls = NULL;
  
  sf_random_t random = random_in;
  sf_list_t die_rolls = NULL;
  for (int i = 0; i < count; ++i) {
    random = sf_random_next_from_range(random, sides);
    die_rolls = sf_list(random, die_rolls);
  }
  dice_throw->die_rolls = sf_retain(die_rolls);
  
  *random_out = random;
  return sf_move_to_temp_pool(dice_throw);
}


int
sj_dice_throw_count(sj_dice_throw_t dice_throw)
{
  return dice_throw ? dice_throw->count : 0;
}


int
sj_dice_throw_sides(sj_dice_throw_t dice_throw)
{
  return dice_throw ? dice_throw->sides : 0;
}


int
sj_dice_throw_total(sj_dice_throw_t dice_throw)
{
  if ( ! dice_throw) return 0;
  
  int total = 0;
  sf_list_t die_rolls = dice_throw->die_rolls;
  while (die_rolls) {
    sf_random_t random = sf_list_head(die_rolls);
    total += sf_random_value(random) + 1;
    die_rolls = sf_list_tail(die_rolls);
  }
  
  for (int i = 0; i < dice_throw->die_modifier_array->count; ++i) {
    total += dice_throw->die_modifier_array->elements[i].value;
  }
  
  return total;
}
