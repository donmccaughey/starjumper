#include "sj_memory.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef VGR_COUNT_ALLOCS

static int alloc_count = 0;

#define INCREMENT_ALLOC_COUNT() (++alloc_count)

#define DECREMENT_ALLOC_COUNT() (--alloc_count)

#define EXPECT_ALLOC_COUNT_ZERO() \
if (alloc_count) { \
fprintf(stderr, "WARNING: %i memory allocations were not freed.\n", alloc_count); \
}

#else

#define INCREMENT_ALLOC_COUNT()
#define DECREMENT_ALLOC_COUNT()
#define EXPECT_ALLOC_COUNT_ZERO()

#endif


static void
fail_and_exit(void)
{
  if (errno) {
    fprintf(stderr, "%s\n", strerror(errno));
    exit(errno);
  } else {
    exit(EXIT_FAILURE);
  }
}


void *
sj_calloc(size_t count, size_t element_size)
{
  void *memory = calloc(count, element_size);
  if ( ! memory) fail_and_exit();
  INCREMENT_ALLOC_COUNT();
  return memory;
}


void *
sj_malloc(size_t size)
{
  void *memory = malloc(size);
  if ( ! memory) fail_and_exit();
  INCREMENT_ALLOC_COUNT();
  return memory;
}


void *
sj_arraydup(void const *memory, size_t count, size_t element_size)
{
  size_t size = count * element_size;
  return sj_memdup(memory, size);
}


void *
sj_memdup(void const *memory, size_t size)
{
  void *dupe = sj_malloc(size);
  memcpy(dupe, memory, size);
  return dupe;
}


void *
sj_realloc(void *memory, size_t size)
{
  void *new_memory = realloc(memory, size);
  if ( ! new_memory) fail_and_exit();
  if ( ! memory) INCREMENT_ALLOC_COUNT();
  return new_memory;
}


void *
sj_reallocarray(void *memory, size_t count, size_t element_size)
{
  size_t size = count * element_size;
  return sj_realloc(memory, size);
}


char *
sj_strdup(char const *string)
{
  char *dupe = strdup(string);
  if ( ! dupe) fail_and_exit();
  INCREMENT_ALLOC_COUNT();
  return dupe;
}



int
sj_asprintf(char **string, char const *format, ...)
{
  va_list arguments;
  va_start(arguments, format);
  int result = sj_vasprintf(string, format, arguments);
  va_end(arguments);
  return result;
}


int
sj_vasprintf(char **string, const char *format, va_list arguments)
{
  int result = vasprintf(string, format, arguments);
  if (-1 == result) fail_and_exit();
  INCREMENT_ALLOC_COUNT();
  return result;
}


void
sj_free(void *memory)
{
  free(memory);
  DECREMENT_ALLOC_COUNT();
}


void
sj_memory_expect_alloc_count_zero(void)
{
  EXPECT_ALLOC_COUNT_ZERO();
}
