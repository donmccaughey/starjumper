#include "sj_die_modifier_array.h"

#include "sj_memory.h"


void
sj_die_modifier_array_add(struct sj_die_modifier_array *die_modifier_array,
                          struct sj_die_modifier die_modifier)
{
  int new_index = die_modifier_array->count;
  int new_count = die_modifier_array->count + 1;
  die_modifier_array->elements = sj_reallocarray(die_modifier_array->elements, new_count, sizeof die_modifier_array->elements[0]);
  die_modifier_array->count = new_count;
  die_modifier_array->elements[new_index] = die_modifier;
}


struct sj_die_modifier_array *
sj_die_modifier_array_alloc_empty(void)
{
  struct sj_die_modifier_array *die_modifier_array = sj_malloc(sizeof(struct sj_die_modifier_array));
  
  die_modifier_array->elements = sj_malloc(0);
  die_modifier_array->count = 0;
  
  return die_modifier_array;
}


struct sj_die_modifier_array *
sj_die_modifier_array_alloc(struct sj_die_modifier const *die_modifiers, int count)
{
  struct sj_die_modifier_array *die_modifier_array = sj_malloc(sizeof(struct sj_die_modifier_array));
  
  die_modifier_array->elements = sj_arraydup(die_modifiers, count, sizeof die_modifiers[0]);
  die_modifier_array->count = count;
  
  return die_modifier_array;
}


void
sj_die_modifier_array_free(struct sj_die_modifier_array *die_modifier_array)
{
  sj_free(die_modifier_array->elements);
  sj_free(die_modifier_array);
}
