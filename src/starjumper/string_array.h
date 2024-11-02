#ifndef STARJUMPER_STRING_ARRAY_H_INCLUDED
#define STARJUMPER_STRING_ARRAY_H_INCLUDED


#include <stddef.h>


typedef char *(*sj_string_array_alloc_string_for_element)(void const *element);


struct sj_string_array
{
  char **elements;
  int count;
};


struct sj_string_array *
sj_string_array_alloc_collect_strings(void const *array,
                                      int count,
                                      size_t element_size,
                                      sj_string_array_alloc_string_for_element alloc_string_for_element);

void
sj_string_array_free(struct sj_string_array *string_array);


#endif
