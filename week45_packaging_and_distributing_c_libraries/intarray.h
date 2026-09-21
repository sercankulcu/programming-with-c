/* intarray.h — do NOT do this in a real library */
typedef struct IntArray {
    int    *data;
    size_t  count;
    size_t  capacity;
} IntArray;

#include <stddef.h>
#include <stdbool.h>
