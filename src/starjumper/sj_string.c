#include "sj_string.h"

#include <string.h>
#include <xmalloc.h>

#include "sj_string_array.h"


char *
sj_string_alloc_join_string_array_with_separator(struct sj_string_array const *string_array,
                                                 char const *separator)
{
  return sj_string_alloc_join_strings_with_separator(
      (char const *const *) string_array->elements,
      string_array->count,
      separator
  );
}


char *
sj_string_alloc_join_string_array_with_suffix(struct sj_string_array const *string_array,
                                              char const *suffix)
{
  return sj_string_alloc_join_strings_with_suffix(
      (char const *const *) string_array->elements,
      string_array->count,
      suffix
  );
}


char *
sj_string_alloc_join_strings_with_separator(char const *const strings[],
                                            int count,
                                            char const *separator)
{
  size_t size = 1;
  size_t separator_size = strlen(separator);
  for (int i = 0; i < count; ++i) {
    if (i) size += separator_size;
    size += strlen(strings[i]);
  }
  char *joined = xmalloc(size);
  char *end = joined;
  for (int i = 0; i < count; ++i) {
    if (i) end = stpcpy(end, separator);
    end = stpcpy(end, strings[i]);
  }
  return joined;
}


char *
sj_string_alloc_join_strings_with_suffix(char const *const strings[],
                                         int count,
                                         char const *suffix)
{
  size_t size = 1;
  size_t suffix_size = strlen(suffix);
  for (int i = 0; i < count; ++i) {
    size += strlen(strings[i]);
    size += suffix_size;
  }
  char *joined = xmalloc(size);
  char *end = joined;
  for (int i = 0; i < count; ++i) {
    end = stpcpy(end, strings[i]);
    end = stpcpy(end, suffix);
  }
  return joined;
}
