/* buffer.h — the caller sees a name, not a layout */
#include <stddef.h>
#include <stdbool.h>

typedef struct Buffer Buffer;      /* incomplete type */

Buffer *buffer_create(size_t capacity);
void    buffer_destroy(Buffer *b);
bool    buffer_append(Buffer *b, int value);
size_t  buffer_count(const Buffer *b);
