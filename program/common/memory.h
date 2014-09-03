#ifndef __MEMORY_H
#define __MEMORY_H

#include <stddef.h>

void *malloc(size_t size);
void free(__attribute__((unused)) void *p);
void *realloc(void *ptr, size_t len);

#endif
