#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- a twenty-line test harness ---------- */

static int tests_run = 0;
static int tests_failed = 0;
static const char *current_suite = "";

#define SUITE(name) do { current_suite = (name); \
    printf("\n-- %s\n", current_suite); } while (0)

#define CHECK(cond) do {                                            \
    tests_run++;                                                    \
    if (cond) {                                                     \
        printf("  ok   %s\n", #cond);                               \
    } else {                                                        \
        tests_failed++;                                             \
        printf("  FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);    \
    }                                                               \
} while (0)

#define CHECK_EQ_INT(actual, expected) do {                         \
    tests_run++;                                                    \
    long a_ = (long)(actual), e_ = (long)(expected);                \
    if (a_ == e_) {                                                 \
        printf("  ok   %s == %ld\n", #actual, e_);                  \
    } else {                                                        \
        tests_failed++;                                             \
        printf("  FAIL %s:%d: %s was %ld, expected %ld\n",          \
               __FILE__, __LINE__, #actual, a_, e_);                \
    }                                                               \
} while (0)

/* ---------- the unit under test: a small string map ---------- */

typedef struct Entry {
    char         *key;
    int           value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry **buckets;
    size_t  bucket_count;
    size_t  count;
} Map;

static size_t hash_string(const char *s)
{
    size_t h = 1469598103934665603u;
    for (; *s != '\0'; s++) { h ^= (unsigned char)*s; h *= 1099511628211u; }
    return h;
}

static Map *map_create(size_t buckets)
{
    if (buckets == 0) buckets = 8;
    Map *m = malloc(sizeof *m);
    if (m == NULL) return NULL;
    m->buckets = calloc(buckets, sizeof *m->buckets);
    if (m->buckets == NULL) { free(m); return NULL; }
    m->bucket_count = buckets;
    m->count = 0;
    return m;
}

static void map_destroy(Map *m)
{
    if (m == NULL) return;
    for (size_t b = 0; b < m->bucket_count; b++) {
        Entry *e = m->buckets[b];
        while (e != NULL) { Entry *n = e->next; free(e->key); free(e); e = n; }
    }
    free(m->buckets);
    free(m);
}

static bool map_put(Map *m, const char *key, int value)
{
    size_t i = hash_string(key) % m->bucket_count;
    for (Entry *e = m->buckets[i]; e != NULL; e = e->next) {
        if (strcmp(e->key, key) == 0) { e->value = value; return true; }
    }
    Entry *e = malloc(sizeof *e);
    if (e == NULL) return false;
    size_t bytes = strlen(key) + 1;
    e->key = malloc(bytes);
    if (e->key == NULL) { free(e); return false; }
    memcpy(e->key, key, bytes);
    e->value = value;
    e->next = m->buckets[i];
    m->buckets[i] = e;
    m->count++;
    return true;
}

static bool map_get(const Map *m, const char *key, int *out)
{
    size_t i = hash_string(key) % m->bucket_count;
    for (Entry *e = m->buckets[i]; e != NULL; e = e->next) {
        if (strcmp(e->key, key) == 0) { *out = e->value; return true; }
    }
    return false;
}

static bool map_remove(Map *m, const char *key)
{
    size_t i = hash_string(key) % m->bucket_count;
    for (Entry **link = &m->buckets[i]; *link != NULL; link = &(*link)->next) {
        if (strcmp((*link)->key, key) == 0) {
            Entry *dead = *link;
            *link = dead->next;
            free(dead->key); free(dead);
            m->count--;
            return true;
        }
    }
    return false;
}

/* ---------- the tests ---------- */

static void test_empty(void)
{
    SUITE("an empty map");
    Map *m = map_create(8);
    int v = -1;
    CHECK(m != NULL);
    CHECK_EQ_INT(m->count, 0);
    CHECK(!map_get(m, "absent", &v));
    CHECK_EQ_INT(v, -1);                  /* untouched on failure */
    CHECK(!map_remove(m, "absent"));
    map_destroy(m);
}

static void test_single(void)
{
    SUITE("one entry");
    Map *m = map_create(8);
    int v = 0;
    CHECK(map_put(m, "key", 42));
    CHECK_EQ_INT(m->count, 1);
    CHECK(map_get(m, "key", &v));
    CHECK_EQ_INT(v, 42);
    CHECK(!map_get(m, "Key", &v));        /* case sensitive */
    map_destroy(m);
}

static void test_replace(void)
{
    SUITE("replacing a value");
    Map *m = map_create(8);
    int v = 0;
    map_put(m, "k", 1);
    CHECK(map_put(m, "k", 2));
    CHECK_EQ_INT(m->count, 1);            /* not 2 */
    map_get(m, "k", &v);
    CHECK_EQ_INT(v, 2);
    map_destroy(m);
}

static void test_collisions(void)
{
    SUITE("forced collisions");
    Map *m = map_create(1);               /* one bucket: everything collides */
    int v = 0;
    for (int i = 0; i < 20; i++) {
        char key[16];
        snprintf(key, sizeof key, "k%d", i);
        map_put(m, key, i);
    }
    CHECK_EQ_INT(m->count, 20);
    CHECK(map_get(m, "k0", &v));   CHECK_EQ_INT(v, 0);
    CHECK(map_get(m, "k19", &v));  CHECK_EQ_INT(v, 19);
    CHECK(map_remove(m, "k10"));
    CHECK(!map_get(m, "k10", &v));
    CHECK_EQ_INT(m->count, 19);
    map_destroy(m);
}

static void test_edge_keys(void)
{
    SUITE("awkward keys");
    Map *m = map_create(8);
    int v = 0;
    CHECK(map_put(m, "", 1));                    /* empty key */
    CHECK(map_get(m, "", &v));   CHECK_EQ_INT(v, 1);

    char long_key[512];
    memset(long_key, 'x', sizeof long_key - 1);
    long_key[sizeof long_key - 1] = '\0';
    CHECK(map_put(m, long_key, 2));
    CHECK(map_get(m, long_key, &v)); CHECK_EQ_INT(v, 2);

    CHECK(map_put(m, "türkçe", 3));              /* multibyte UTF-8 */
    CHECK(map_get(m, "türkçe", &v)); CHECK_EQ_INT(v, 3);
    map_destroy(m);
}

static void test_remove_order(void)
{
    SUITE("removal from every chain position");
    const char *keys[] = { "a", "b", "c" };
    for (int victim = 0; victim < 3; victim++) {
        Map *m = map_create(1);                  /* all in one chain */
        for (int i = 0; i < 3; i++) map_put(m, keys[i], i);
        CHECK(map_remove(m, keys[victim]));
        CHECK_EQ_INT(m->count, 2);
        int v;
        for (int i = 0; i < 3; i++) {
            if (i == victim) CHECK(!map_get(m, keys[i], &v));
            else             CHECK(map_get(m, keys[i], &v));
        }
        map_destroy(m);
    }
}

int main(void)
{
    puts("running tests");
    test_empty();
    test_single();
    test_replace();
    test_collisions();
    test_edge_keys();
    test_remove_order();

    printf("\n%d checks, %d failed\n", tests_run, tests_failed);
    return tests_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
