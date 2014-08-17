#ifndef STARJUMPER_SJ_DIE_ROLL_H_INCLUDED
#define STARJUMPER_SJ_DIE_ROLL_H_INCLUDED


#include <starjumper/sj_defs.h>


struct sj_die_modifier_array;
struct sj_random;


struct sj_dice_throw
{
  int *die_rolls;
  int count;
  int sides;
  int *modifiers;
  int modifiers_count;
};


struct sj_dice_throw *
sj_dice_throw_alloc(int count,
                    int sides,
                    struct sj_die_modifier_array *die_modifier_array,
                    struct sj_random *random);

void
sj_dice_throw_free(struct sj_dice_throw *dice_throw);

int
sj_dice_throw_total(struct sj_dice_throw *dice_throw);

char *
sj_string_alloc_from_dice_throw(struct sj_dice_throw const *dice_throw);


#endif
