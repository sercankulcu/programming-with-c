/* intarray.h — a growable array of int.
 *
 * Error contract:
 *   Functions returning bool give true on success, false on failure;
 *   on failure the array is left unchanged and usable.
 *
 * Ownership:
 *   intarray_create returns a handle the caller must pass to
 *   intarray_destroy exactly once. No other function frees anything.
 */
#ifndef INTARRAY_H
#define INTARRAY_H

#include <stddef.h>
#include <stdbool.h>

typedef struct IntArray IntArray;     /* opaque: no layout exposed */

IntArray *intarray_create(size_t initial_capacity);
void      intarray_destroy(IntArray *a);

bool      intarray_push(IntArray *a, int value);
bool      intarray_get(const IntArray *a, size_t index, int *out);
size_t    intarray_count(const IntArray *a);
size_t    intarray_capacity(const IntArray *a);
void      intarray_clear(IntArray *a);

#endif /* INTARRAY_H */
