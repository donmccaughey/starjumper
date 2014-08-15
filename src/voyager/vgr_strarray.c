#include "vgr_strarray.h"

#include <stddef.h>

#include "vgr_memory.h"


struct vgr_strarray *
vgr_strarray_alloc_collect_strings(void const *source,
                                   int count,
                                   size_t item_size,
                                   vgr_strarray_alloc_string_for_item alloc_string_for_item)
{
  struct vgr_strarray *strarray = vgr_malloc(sizeof(struct vgr_strarray));
  strarray->strings = vgr_calloc(count + 1, sizeof(char *));
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
vgr_strarray_free(struct vgr_strarray *strarray)
{
  for (int i = 0; i < strarray->count; ++i) {
    vgr_free(strarray->strings[i]);
  }
  vgr_free(strarray->strings);
  vgr_free(strarray);
}
