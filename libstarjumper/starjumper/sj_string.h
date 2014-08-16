#ifndef STARJUMPER_SJ_STRING_H_INCLUDED
#define STARJUMPER_SJ_STRING_H_INCLUDED


#include <stddef.h>


struct sj_strarray;


char *
sj_string_alloc_join_strarray_with_suffix(struct sj_strarray const *strarray,
                                          char const *suffix);

char *
sj_string_alloc_join_strings_with_suffix(char const *const strings[],
                                         int count,
                                         char const *suffix);


#endif
