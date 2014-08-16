#ifndef STARJUMPER_SJ_STRARRAY_H_INCLUDED
#define STARJUMPER_SJ_STRARRAY_H_INCLUDED


#include <stddef.h>


typedef char *(*sj_strarray_alloc_string_for_item)(void const *item);


struct sj_strarray
{
  char **strings;
  int count;
};


struct sj_strarray *
sj_strarray_alloc_collect_strings(void const *source,
                                  int count,
                                  size_t item_size,
                                  sj_strarray_alloc_string_for_item alloc_string_for_item);

void
sj_strarray_free(struct sj_strarray *strarray);


#endif
