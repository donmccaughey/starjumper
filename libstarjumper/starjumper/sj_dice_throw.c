#include "sj_dice_throw.h"

#include <alloc_or_die.h>
#include <rnd.h>

#include "sj_string.h"
#include "sj_string_array.h"


static char *
alloc_modifier_string(void const *item);

static char *
alloc_roll_string(void const *item);


static char *
alloc_modifier_string(void const *item)
{
  int const *modifier = item;
  char *string;
  asprintf_or_die(&string, "%+i", *modifier);
  return string;
}


static char *
alloc_roll_string(void const *item)
{
  int const *roll = item;
  char *string;
  asprintf_or_die(&string, "%i", *roll);
  return string;
}


int
sj_dice_throw(int count,
              int sides,
              int modifiers[],
              int modifiers_count,
              struct rnd *rnd)
{
  struct sj_dice_throw *dice_throw = sj_dice_throw_alloc(count, sides, rnd);
  for (int i = 0; i < modifiers_count; ++i) {
    sj_dice_throw_add_modifier(dice_throw, modifiers[i]);
  }
  int total = sj_dice_throw_total(dice_throw);
  sj_dice_throw_free(dice_throw);
  return total;
}


void
sj_dice_throw_add_modifier(struct sj_dice_throw *dice_throw, int modifier)
{
  int next_index = dice_throw->modifiers_count;
  ++dice_throw->modifiers_count;
  dice_throw->modifiers = reallocarray_or_die(dice_throw->modifiers,
                                          dice_throw->modifiers_count,
                                          sizeof dice_throw->modifiers[0]);
  dice_throw->modifiers[next_index] = modifier;
}


struct sj_dice_throw *
sj_dice_throw_alloc(int count,
                    int sides,
                    struct rnd *rnd)
{
  struct sj_dice_throw *dice_throw = malloc_or_die(sizeof(struct sj_dice_throw));
  
  dice_throw->count = count;
  dice_throw->sides = sides;
  
  dice_throw->rolls = malloc_or_die(count * sizeof(int));
  for (int i = 0; i < count; ++i) {
    dice_throw->rolls[i] = rnd_next_uniform_value_in_range(rnd, 1, sides);
  }
  
  dice_throw->modifiers = NULL;
  dice_throw->modifiers_count = 0;
  
  return dice_throw;
}


void
sj_dice_throw_free(struct sj_dice_throw *dice_throw)
{
  free_or_die(dice_throw->rolls);
  free_or_die(dice_throw->modifiers);
  free_or_die(dice_throw);
}


int
sj_dice_throw_total(struct sj_dice_throw *dice_throw)
{
  int total = 0;
  for (int i = 0; i < dice_throw->count; ++i) {
    total += dice_throw->rolls[i];
  }
  
  for (int i = 0; i < dice_throw->modifiers_count; ++i) {
    total += dice_throw->modifiers[i];
  }
  
  return total;
}


char *
sj_string_alloc_from_dice_throw(struct sj_dice_throw const *dice_throw)
{
  char *dice;
  asprintf_or_die(&dice, "%iD%i", dice_throw->count, dice_throw->sides);
  
  struct sj_string_array *rolls_array = sj_string_array_alloc_collect_strings(
      dice_throw->rolls,
      dice_throw->count,
      sizeof dice_throw->rolls[0],
      alloc_roll_string
  );
  char *rolls = sj_string_alloc_join_string_array_with_separator(rolls_array, ", ");
  sj_string_array_free(rolls_array);
  
  struct sj_string_array *modifiers_array = sj_string_array_alloc_collect_strings(
      dice_throw->modifiers,
      dice_throw->modifiers_count,
      sizeof dice_throw->modifiers[0],
      alloc_modifier_string
  );
  char *modifiers = sj_string_alloc_join_string_array_with_separator(modifiers_array, ", ");
  sj_string_array_free(modifiers_array);
  
  char *string;
  asprintf_or_die(&string, "%s (%s) %s", dice, rolls, modifiers);
  free_or_die(modifiers);
  free_or_die(rolls);
  free_or_die(dice);
  return string;
}
