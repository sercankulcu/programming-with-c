/* buffer.c — the definition lives here and nowhere else */
struct Buffer {
    int    *data;
    size_t  count;
    size_t  capacity;
};
