/* map.h — the caller sees no implementation at all */
typedef struct Map Map;

Map   *map_create(void);
void   map_destroy(Map *m);
bool   map_put(Map *m, const char *key, int value);
bool   map_get(const Map *m, const char *key, int *out);
bool   map_remove(Map *m, const char *key);
size_t map_count(const Map *m);
void   map_each(const Map *m, void (*visit)(const char *, int, void *), void *ctx);
