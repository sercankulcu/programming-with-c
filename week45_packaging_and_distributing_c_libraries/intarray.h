#ifndef INTARRAY_H
#define INTARRAY_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {                          /* week 46: C++ callers */
#endif

#define INTARRAY_VERSION_MAJOR 2
#define INTARRAY_VERSION_MINOR 3
#define INTARRAY_VERSION_PATCH 0

typedef struct IntArray IntArray;     /* opaque: no layout promised */

IntArray *intarray_create(size_t initial_capacity);
void      intarray_destroy(IntArray *a);
bool      intarray_push(IntArray *a, int value);
size_t    intarray_count(const IntArray *a);

const char *intarray_version_string(void);

#ifdef __cplusplus
}
#endif

#endif /* INTARRAY_H */
