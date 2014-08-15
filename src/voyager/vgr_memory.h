#ifndef VOYAGER_VGR_MEMORY_H_INCLUDED
#define VOYAGER_VGR_MEMORY_H_INCLUDED


#include <stdarg.h>
#include <stddef.h>


void *
vgr_malloc(size_t size);

void *
vgr_memdup(void const *memory, size_t size);

void *
vgr_realloc(void *memory, size_t size);

char *
vgr_strdup(char const *string);

int
vgr_asprintf(char **string, char const *format, ...);

int
vgr_vasprintf(char **string, const char *format, va_list arguments);

void
vgr_free(void *memory);

void
vgr_memory_expect_alloc_count_zero(void);


#endif
