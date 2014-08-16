#ifndef STARJUMPER_SJ_DIE_ROLL_H_INCLUDED
#define STARJUMPER_SJ_DIE_ROLL_H_INCLUDED


#include <starjumper/sj_defs.h>


extern sf_type_t sj_dice_throw_type;


sj_dice_throw_t
sj_dice_throw(int count,
              int sides,
              sf_list_t die_modifiers,
              sf_random_t random_in,
              sf_random_t *random_out);

int
sj_dice_throw_count(sj_dice_throw_t dice_throw);

sf_list_t
sj_dice_throw_modifiers(sj_dice_throw_t dice_throw);

int
sj_dice_throw_sides(sj_dice_throw_t dice_throw);

int
sj_dice_throw_total(sj_dice_throw_t dice_throw);


#endif
