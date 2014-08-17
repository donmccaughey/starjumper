#ifndef STARJUMPER_SJ_DIE_MODIFIER_H_INCLUDED
#define STARJUMPER_SJ_DIE_MODIFIER_H_INCLUDED


struct sj_die_modifier
{
  int value;
};


char *
sj_string_alloc_from_die_modifier(struct sj_die_modifier die_modifier);


#endif
