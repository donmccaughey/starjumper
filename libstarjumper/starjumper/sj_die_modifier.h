#ifndef STARJUMPER_SJ_DIE_MODIFIER_H_INCLUDED
#define STARJUMPER_SJ_DIE_MODIFIER_H_INCLUDED


#include <starjumper/sj_defs.h>


extern sf_type_t sj_die_modifier_type;


sj_die_modifier_t
sj_die_modifier(int value);

int
sj_die_modifier_value(sj_die_modifier_t die_modifier);


#endif
