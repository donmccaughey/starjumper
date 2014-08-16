#ifndef STARJUMPER_SJ_MEMORY_H_INCLUDED
#define STARJUMPER_SJ_MEMORY_H_INCLUDED


#include <stdarg.h>
#include <stddef.h>


void *
sj_calloc(size_t count, size_t size);

void *
sj_malloc(size_t size);

void *
sj_memdup(void const *memory, size_t size);

void *
sj_realloc(void *memory, size_t size);

char *
sj_strdup(char const *string);

int
sj_asprintf(char **string, char const *format, ...);

int
sj_vasprintf(char **string, const char *format, va_list arguments);

void
sj_free(void *memory);

void
sj_memory_expect_alloc_count_zero(void);


#endif
