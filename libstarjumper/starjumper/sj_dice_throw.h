#ifndef STARJUMPER_SJ_DIE_ROLL_H_INCLUDED
#define STARJUMPER_SJ_DIE_ROLL_H_INCLUDED


#include <starjumper/sj_defs.h>


struct sj_die_modifier_array;
struct sj_random;


extern sf_type_t sj_dice_throw_type;


sj_dice_throw_t
sj_dice_throw(int count,
              int sides,
              struct sj_die_modifier_array *die_modifier_array,
              struct sj_random *random);

int
sj_dice_throw_count(sj_dice_throw_t dice_throw);

int
sj_dice_throw_sides(sj_dice_throw_t dice_throw);

int
sj_dice_throw_total(sj_dice_throw_t dice_throw);


#endif