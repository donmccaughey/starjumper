#ifndef VOYAGER_VGR_STRING_H_INCLUDED
#define VOYAGER_VGR_STRING_H_INCLUDED


#include <stddef.h>


struct vgr_strarray;


char *
vgr_string_alloc_join_strarray_with_suffix(struct vgr_strarray const *strarray,
                                           char const *suffix);

char *
vgr_string_alloc_join_strings_with_suffix(char const *const strings[],
                                          int count,
                                          char const *suffix);


#endif
