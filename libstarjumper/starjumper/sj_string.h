#ifndef STARJUMPER_SJ_STRING_H_INCLUDED
#define STARJUMPER_SJ_STRING_H_INCLUDED


#include <stddef.h>


struct sj_string_array;


char *
sj_string_alloc_join_string_array_with_suffix(struct sj_string_array const *string_array,
                                              char const *suffix);

char *
sj_string_alloc_join_strings_with_suffix(char const *const strings[],
                                         int count,
                                         char const *suffix);


#endif
