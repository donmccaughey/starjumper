#ifndef STARJUMPER_SJ_DIE_MODIFIER_ARRAY_H_INCLUDED
#define STARJUMPER_SJ_DIE_MODIFIER_ARRAY_H_INCLUDED


#include <starjumper/sj_die_modifier.h>


struct sj_die_modifier_array
{
  struct sj_die_modifier *elements;
  int count;
};


void
sj_die_modifier_array_add(struct sj_die_modifier_array *die_modifier_array,
                          struct sj_die_modifier die_modifier);

struct sj_die_modifier_array *
sj_die_modifier_array_alloc_empty(void);

struct sj_die_modifier_array *
sj_die_modifier_array_alloc(struct sj_die_modifier const *die_modifiers, int count);

void
sj_die_modifier_array_free(struct sj_die_modifier_array *die_modifier_array);


#endif
