#ifndef STARJUMPER_VGR_DIE_MODIFIER_H_INCLUDED
#define STARJUMPER_VGR_DIE_MODIFIER_H_INCLUDED


#include <starjumper/vgr_defs.h>


extern sf_type_t vgr_die_modifier_type;


vgr_die_modifier_t
vgr_die_modifier(int value);

int
vgr_die_modifier_value(vgr_die_modifier_t die_modifier);


#endif
