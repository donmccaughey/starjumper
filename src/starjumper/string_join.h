#ifndef STARJUMPER_STRING_H_INCLUDED
#define STARJUMPER_STRING_H_INCLUDED


#include <stddef.h>


struct sj_string_array;


char *
sj_string_alloc_join_string_array_with_separator(struct sj_string_array const *string_array,
                                                 char const *separator);

char *
sj_string_alloc_join_string_array_with_suffix(struct sj_string_array const *string_array,
                                              char const *suffix);

char *
sj_string_alloc_join_strings_with_separator(char const *const strings[],
                                            int count,
                                            char const *separator);

char *
sj_string_alloc_join_strings_with_suffix(char const *const strings[],
                                         int count,
                                         char const *suffix);


#endif
