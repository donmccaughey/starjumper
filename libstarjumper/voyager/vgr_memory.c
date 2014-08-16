#include "vgr_memory.h"

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
vgr_calloc(size_t count, size_t size)
{
  void *memory = calloc(count, size);
  if ( ! memory) fail_and_exit();
  INCREMENT_ALLOC_COUNT();
  return memory;
}


void *
vgr_malloc(size_t size)
{
  void *memory = malloc(size);
  if ( ! memory) fail_and_exit();
  INCREMENT_ALLOC_COUNT();
  return memory;
}


void *
vgr_memdup(void const *memory, size_t size)
{
  void *dupe = vgr_malloc(size);
  memcpy(dupe, memory, size);
  return dupe;
}


void *
vgr_realloc(void *memory, size_t size)
{
  void *new_memory = realloc(memory, size);
  if ( ! new_memory) fail_and_exit();
  if ( ! memory) INCREMENT_ALLOC_COUNT();
  return new_memory;
}


char *
vgr_strdup(char const *string)
{
  char *dupe = strdup(string);
  if ( ! dupe) fail_and_exit();
  INCREMENT_ALLOC_COUNT();
  return dupe;
}



int
vgr_asprintf(char **string, char const *format, ...)
{
  va_list arguments;
  va_start(arguments, format);
  int result = vgr_vasprintf(string, format, arguments);
  va_end(arguments);
  return result;
}


int
vgr_vasprintf(char **string, const char *format, va_list arguments)
{
  int result = vasprintf(string, format, arguments);
  if (-1 == result) fail_and_exit();
  INCREMENT_ALLOC_COUNT();
  return result;
}


void
vgr_free(void *memory)
{
  free(memory);
  DECREMENT_ALLOC_COUNT();
}


void
vgr_memory_expect_alloc_count_zero(void)
{
  EXPECT_ALLOC_COUNT_ZERO();
}
