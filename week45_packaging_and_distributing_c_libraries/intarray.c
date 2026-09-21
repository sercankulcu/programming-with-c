#include "intarray.h"

#include <stdlib.h>
#include <assert.h>

#define DEFAULT_CAPACITY 4          /* internal: not in the header */

struct IntArray {                   /* the definition lives only here */
    int    *data;
    size_t  count;
    size_t  capacity;
};

/* static: invisible outside this file, no link-time collisions */
static bool grow(IntArray *a)
{
    size_t bigger = a->capacity * 2;
    int *moved = realloc(a->data, bigger * sizeof *moved);
    if (moved == NULL) {
        return false;               /* a->data still valid */
    }
    a->data = moved;
    a->capacity = bigger;
    return true;
}

IntArray *intarray_create(size_t initial_capacity)
{
    if (initial_capacity == 0) {
        initial_capacity = DEFAULT_CAPACITY;
    }

    IntArray *a = malloc(sizeof *a);
    if (a == NULL) {
        return NULL;
    }
    a->data = malloc(initial_capacity * sizeof *a->data);
    if (a->data == NULL) {
        free(a);                    /* release what we got */
        return NULL;
    }
    a->count = 0;
    a->capacity = initial_capacity;
    return a;
}

void intarray_destroy(IntArray *a)
{
    if (a == NULL) {
        return;                     /* destroy(NULL) is safe, like free */
    }
    free(a->data);
    free(a);
}

bool intarray_push(IntArray *a, int value)
{
    assert(a != NULL);
    if (a->count == a->capacity && !grow(a)) {
        return false;
    }
    a->data[a->count++] = value;
    return true;
}

bool intarray_get(const IntArray *a, size_t index, int *out)
{
    assert(a != NULL && out != NULL);
    if (index >= a->count) {
        return false;
    }
    *out = a->data[index];
    return true;
}

size_t intarray_count(const IntArray *a)    { assert(a); return a->count; }
size_t intarray_capacity(const IntArray *a) { assert(a); return a->capacity; }
void   intarray_clear(IntArray *a)          { assert(a); a->count = 0; }

/* Part of the installed interface: lets a caller check at run time what it
   is actually linked against, which may not be what it compiled against. */
const char *intarray_version_string(void)
{
    return "2.3.0";
}
