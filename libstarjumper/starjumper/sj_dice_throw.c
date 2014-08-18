#include "sj_dice_throw.h"

#include "sj_memory.h"
#include "sj_random.h"
#include "sj_string.h"
#include "sj_string_array.h"


static char *
alloc_die_modifier_string(void const *item);

static char *
alloc_die_roll_string(void const *item);


static char *
alloc_die_modifier_string(void const *item)
{
  int const *modifier = item;
  char *string;
  sj_asprintf(&string, "%+i", *modifier);
  return string;
}


static char *
alloc_die_roll_string(void const *item)
{
  int const *die_roll = item;
  char *string;
  sj_asprintf(&string, "%i", *die_roll);
  return string;
}


void
sj_dice_throw_add_modifier(struct sj_dice_throw *dice_throw, int modifier)
{
  int next_index = dice_throw->modifiers_count;
  ++dice_throw->modifiers_count;
  dice_throw->modifiers = sj_reallocarray(dice_throw->modifiers,
                                          dice_throw->modifiers_count,
                                          sizeof dice_throw->modifiers[0]);
  dice_throw->modifiers[next_index] = modifier;
}


struct sj_dice_throw *
sj_dice_throw_alloc(int count,
                    int sides,
                    struct sj_random *random)
{
  struct sj_dice_throw *dice_throw = sj_malloc(sizeof(struct sj_dice_throw));
  
  dice_throw->count = count;
  dice_throw->sides = sides;
  
  dice_throw->die_rolls = sj_malloc(count * sizeof(int));
  for (int i = 0; i < count; ++i) {
    dice_throw->die_rolls[i] = 1 + sj_random_next_value_in_range(random, sides);
  }
  
  dice_throw->modifiers = NULL;
  dice_throw->modifiers_count = 0;
  
  return dice_throw;
}


void
sj_dice_throw_free(struct sj_dice_throw *dice_throw)
{
  sj_free(dice_throw->die_rolls);
  sj_free(dice_throw->modifiers);
  sj_free(dice_throw);
}


int
sj_dice_throw_total(struct sj_dice_throw *dice_throw)
{
  int total = 0;
  for (int i = 0; i < dice_throw->count; ++i) {
    total += dice_throw->die_rolls[i];
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
  sj_asprintf(&dice, "%iD%i", dice_throw->count, dice_throw->sides);
  
  struct sj_string_array *die_rolls_array = sj_string_array_alloc_collect_strings(
      dice_throw->die_rolls,
      dice_throw->count,
      sizeof dice_throw->die_rolls[0],
      alloc_die_roll_string
  );
  char *die_rolls = sj_string_alloc_join_string_array_with_separator(die_rolls_array, ", ");
  sj_string_array_free(die_rolls_array);
  
  struct sj_string_array *die_modifiers_array = sj_string_array_alloc_collect_strings(
      dice_throw->modifiers,
      dice_throw->modifiers_count,
      sizeof dice_throw->modifiers[0],
      alloc_die_modifier_string
  );
  char *die_modifiers = sj_string_alloc_join_string_array_with_separator(die_modifiers_array, ", ");
  sj_string_array_free(die_modifiers_array);
  
  char *string;
  sj_asprintf(&string, "%s (%s) %s", dice, die_rolls, die_modifiers);
  return string;
}
