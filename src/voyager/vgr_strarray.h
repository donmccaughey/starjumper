#ifndef VOYAGER_VGR_STRARRAY_H_INCLUDED
#define VOYAGER_VGR_STRARRAY_H_INCLUDED


#include <stddef.h>


typedef char *(*vgr_strarray_alloc_string_for_item)(void const *item);


struct vgr_strarray
{
  char **strings;
  int count;
};


struct vgr_strarray *
vgr_strarray_alloc_collect_strings(void const *source,
                                   int count,
                                   size_t item_size,
                                   vgr_strarray_alloc_string_for_item alloc_string_for_item);

void
vgr_strarray_free(struct vgr_strarray *strarray);


#endif
