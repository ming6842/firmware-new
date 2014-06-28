#ifndef __stdlib_H
#define __stdlib_H

#include <stddef.h>

void *malloc(size_t size);
void free(void *p);
void *realloc(void *ptr, size_t len);

#endif
