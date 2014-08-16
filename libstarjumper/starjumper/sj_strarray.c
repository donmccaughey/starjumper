#include "sj_strarray.h"

#include <stddef.h>

#include "sj_memory.h"


struct sj_strarray *
sj_strarray_alloc_collect_strings(void const *source,
                                  int count,
                                  size_t item_size,
                                  sj_strarray_alloc_string_for_item alloc_string_for_item)
{
  struct sj_strarray *strarray = sj_malloc(sizeof(struct sj_strarray));
  strarray->strings = sj_calloc(count + 1, sizeof(char *));
  strarray->count = (int) count;
  
  for (size_t i = 0; i < count; ++i) {
    size_t item_offset = item_size * i;
    void const *item = source + item_offset;
    strarray->strings[i] = alloc_string_for_item(item);
  }
  
  size_t null_terminator_index = count;
  strarray->strings[null_terminator_index] = NULL;
  
  return strarray;
}


void
sj_strarray_free(struct sj_strarray *strarray)
{
  for (int i = 0; i < strarray->count; ++i) {
    sj_free(strarray->strings[i]);
  }
  sj_free(strarray->strings);
  sj_free(strarray);
}
