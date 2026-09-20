#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/* ================= binary search tree ================= */

typedef struct TreeNode {
    int              key;
    struct TreeNode *left, *right;
} TreeNode;

static bool tree_insert(TreeNode **link, int key)
{
    while (*link != NULL) {
        if      (key < (*link)->key) link = &(*link)->left;
        else if (key > (*link)->key) link = &(*link)->right;
        else return false;
    }
    TreeNode *n = malloc(sizeof *n);
    if (n == NULL) return false;
    n->key = key;
    n->left = n->right = NULL;
    *link = n;
    return true;
}

static bool tree_contains(const TreeNode *n, int key, int *steps)
{
    while (n != NULL) {
        (*steps)++;
        if      (key < n->key) n = n->left;
        else if (key > n->key) n = n->right;
        else return true;
    }
    return false;
}

static void tree_inorder(const TreeNode *n, void (*visit)(int, void *), void *ctx)
{
    if (n == NULL) return;
    tree_inorder(n->left, visit, ctx);      /* recursion from week 18 */
    visit(n->key, ctx);
    tree_inorder(n->right, visit, ctx);
}

/* Post-order: children must be freed before their parent. */
static void tree_destroy(TreeNode *n)
{
    if (n == NULL) return;
    tree_destroy(n->left);
    tree_destroy(n->right);
    free(n);
}

static int tree_height(const TreeNode *n)
{
    if (n == NULL) return 0;
    int l = tree_height(n->left), r = tree_height(n->right);
    return 1 + (l > r ? l : r);
}

/* ================= hash map, behind an opaque handle ================= */

typedef struct Entry {
    char         *key;
    int           value;
    struct Entry *next;
} Entry;

struct Map {                              /* defined here only */
    Entry **buckets;
    size_t  bucket_count;
    size_t  count;
};
typedef struct Map Map;

static size_t hash_string(const char *s)
{
    size_t h = 1469598103934665603u;
    for (; *s != '\0'; s++) {
        h ^= (unsigned char)*s;
        h *= 1099511628211u;
    }
    return h;
}

static Map *map_create(void)
{
    Map *m = malloc(sizeof *m);
    if (m == NULL) return NULL;
    m->bucket_count = 8;
    m->buckets = calloc(m->bucket_count, sizeof *m->buckets);
    if (m->buckets == NULL) { free(m); return NULL; }
    m->count = 0;
    return m;
}

static void map_destroy(Map *m)
{
    if (m == NULL) return;
    for (size_t b = 0; b < m->bucket_count; b++) {
        Entry *e = m->buckets[b];
        while (e != NULL) {
            Entry *next = e->next;        /* save before freeing */
            free(e->key);
            free(e);
            e = next;
        }
    }
    free(m->buckets);
    free(m);
}

/* Every key's index depends on bucket_count, so all must be rehashed. */
static bool map_grow(Map *m)
{
    size_t new_count = m->bucket_count * 2;
    Entry **fresh = calloc(new_count, sizeof *fresh);
    if (fresh == NULL) return false;

    for (size_t b = 0; b < m->bucket_count; b++) {
        Entry *e = m->buckets[b];
        while (e != NULL) {
            Entry *next = e->next;
            size_t i = hash_string(e->key) % new_count;
            e->next = fresh[i];
            fresh[i] = e;
            e = next;
        }
    }
    free(m->buckets);
    m->buckets = fresh;
    m->bucket_count = new_count;
    return true;
}

static bool map_put(Map *m, const char *key, int value)
{
    size_t i = hash_string(key) % m->bucket_count;
    for (Entry *e = m->buckets[i]; e != NULL; e = e->next) {
        if (strcmp(e->key, key) == 0) {
            e->value = value;             /* replace */
            return true;
        }
    }

    if ((m->count + 1) * 4 > m->bucket_count * 3) {   /* load factor 0.75 */
        if (!map_grow(m)) return false;
        i = hash_string(key) % m->bucket_count;
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

static bool map_get(const Map *m, const char *key, int *out, int *steps)
{
    size_t i = hash_string(key) % m->bucket_count;
    for (Entry *e = m->buckets[i]; e != NULL; e = e->next) {
        if (steps) (*steps)++;
        if (strcmp(e->key, key) == 0) {
            *out = e->value;
            return true;
        }
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
            free(dead->key);
            free(dead);
            m->count--;
            return true;
        }
    }
    return false;
}

static void map_stats(const Map *m)
{
    size_t used = 0, longest = 0;
    for (size_t b = 0; b < m->bucket_count; b++) {
        size_t len = 0;
        for (const Entry *e = m->buckets[b]; e != NULL; e = e->next) len++;
        if (len > 0) used++;
        if (len > longest) longest = len;
    }
    printf("  %zu entries in %zu buckets, %zu used, longest chain %zu, load %.2f\n",
           m->count, m->bucket_count, used, longest,
           (double)m->count / (double)m->bucket_count);
}

/* ================= visitors ================= */

static void print_key(int key, void *ctx)
{
    (void)ctx;
    printf("%d ", key);
}

static void count_key(int key, void *ctx)
{
    (void)key;
    (*(long *)ctx)++;
}

int main(void)
{
    puts("== binary search tree ==");
    TreeNode *root = NULL;
    int keys[] = { 50, 30, 70, 20, 40, 60, 80 };
    for (size_t i = 0; i < 7; i++) tree_insert(&root, keys[i]);

    printf("  in-order (sorted): ");
    tree_inorder(root, print_key, NULL);
    printf("\n  height = %d for 7 keys (log2(7) = 2.8)\n", tree_height(root));

    int steps = 0;
    printf("  contains 40: %s in %d comparisons\n",
           tree_contains(root, 40, &steps) ? "yes" : "no", steps);
    steps = 0;
    printf("  contains 45: %s in %d comparisons\n",
           tree_contains(root, 45, &steps) ? "yes" : "no", steps);

    long visited = 0;
    tree_inorder(root, count_key, &visited);
    printf("  visitor counted %ld nodes\n", visited);
    tree_destroy(root);

    puts("\n== the degenerate case ==");
    TreeNode *sorted_root = NULL;
    for (int i = 1; i <= 1000; i++) tree_insert(&sorted_root, i);
    printf("  1000 keys inserted in sorted order: height = %d\n",
           tree_height(sorted_root));
    steps = 0;
    tree_contains(sorted_root, 1000, &steps);
    printf("  finding the last key took %d comparisons, not ~10\n", steps);
    puts("  the tree became a linked list; this is why balancing exists");
    tree_destroy(sorted_root);

    TreeNode *balanced = NULL;
    int mid[] = { 500, 250, 750, 125, 375, 625, 875 };
    for (size_t i = 0; i < 7; i++) tree_insert(&balanced, mid[i]);
    for (int i = 1; i <= 1000; i++) tree_insert(&balanced, i);
    printf("  same keys, better insertion order: height = %d\n",
           tree_height(balanced));
    tree_destroy(balanced);

    puts("\n== hash map behind an opaque handle ==");
    Map *m = map_create();
    if (m == NULL) return EXIT_FAILURE;

    const char *words[] = { "alpha","beta","gamma","delta","epsilon",
                            "zeta","eta","theta","iota","kappa" };
    for (int i = 0; i < 10; i++) map_put(m, words[i], i * 10);

    map_stats(m);

    int value;
    steps = 0;
    printf("  get \"gamma\": %s = %d in %d probes\n",
           map_get(m, "gamma", &value, &steps) ? "found" : "missing",
           value, steps);
    printf("  get \"omega\": %s\n",
           map_get(m, "omega", &value, NULL) ? "found" : "missing");

    map_put(m, "gamma", 999);
    map_get(m, "gamma", &value, NULL);
    printf("  after replacing: gamma = %d, count still %zu\n",
           value, map_count_placeholder(m));

    printf("  remove \"beta\": %s\n", map_remove(m, "beta") ? "yes" : "no");
    map_stats(m);
    map_destroy(m);

    puts("\n== growth and rehashing ==");
    Map *big = map_create();
    char key[32];
    for (int i = 0; i < 100; i++) {
        snprintf(key, sizeof key, "key%03d", i);
        map_put(big, key, i);
    }
    map_stats(big);
    puts("  buckets doubled from 8 as the load factor crossed 0.75;");
    puts("  every entry was rehashed, because the index depends on the count");

    clock_t t0 = clock();
    long hits = 0;
    for (int pass = 0; pass < 20000; pass++) {
        snprintf(key, sizeof key, "key%03d", pass % 100);
        if (map_get(big, key, &value, NULL)) hits++;
    }
    printf("  20000 lookups in %.4f s, %ld hits\n",
           (double)(clock() - t0) / CLOCKS_PER_SEC, hits);
    map_destroy(big);

    return EXIT_SUCCESS;
}
