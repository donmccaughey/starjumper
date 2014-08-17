#include "sj_die_modifier.h"

#include "sj_memory.h"


char *
sj_string_alloc_from_die_modifier(struct sj_die_modifier const die_modifier)
{
  char *string;
  sj_asprintf(&string, "%+i", die_modifier.value);
  return string;
}
