#include "vgr_string.h"

#include <string.h>

#include "vgr_memory.h"
#include "vgr_strarray.h"



char *
vgr_string_alloc_join_strarray_with_suffix(struct vgr_strarray const *strarray,
                                           char const *suffix)
{
  return vgr_string_alloc_join_strings_with_suffix((char const *const *) strarray->strings, strarray->count, suffix);
}


char *
vgr_string_alloc_join_strings_with_suffix(char const *const strings[],
                                          int count,
                                          char const *suffix)
{
  size_t size = 1;
  size_t suffix_size = strlen(suffix);
  for (int i = 0; i < count; ++i) {
    size += strlen(strings[i]);
    size += suffix_size;
  }
  char *joined = vgr_malloc(size);
  char *end = joined;
  for (int i = 0; i < count; ++i) {
    end = stpcpy(end, strings[i]);
    end = stpcpy(end, suffix);
  }
  return joined;
}
