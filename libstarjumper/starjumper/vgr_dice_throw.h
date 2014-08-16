#ifndef STARJUMPER_VGR_DIE_ROLL_H_INCLUDED
#define STARJUMPER_VGR_DIE_ROLL_H_INCLUDED


#include <starjumper/vgr_defs.h>


extern sf_type_t vgr_dice_throw_type;


vgr_dice_throw_t
vgr_dice_throw(int count,
               int sides,
               sf_list_t die_modifiers,
               sf_random_t random_in,
               sf_random_t *random_out);

int
vgr_dice_throw_count(vgr_dice_throw_t dice_throw);

sf_list_t
vgr_dice_throw_modifiers(vgr_dice_throw_t dice_throw);

int
vgr_dice_throw_sides(vgr_dice_throw_t dice_throw);

int
vgr_dice_throw_total(vgr_dice_throw_t dice_throw);


#endif
