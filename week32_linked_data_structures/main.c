#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* ================= singly linked list ================= */

typedef struct Node {
    int          value;
    struct Node *next;
} Node;

typedef struct {
    Node   *head;
    Node   *tail;          /* for O(1) append and enqueue */
    size_t  count;
} List;

static void list_init(List *l)
{
    l->head = l->tail = NULL;
    l->count = 0;
}

static void list_destroy(List *l)
{
    Node *n = l->head;
    while (n != NULL) {
        Node *next = n->next;      /* save before freeing */
        free(n);
        n = next;
    }
    list_init(l);
}

static bool list_push_front(List *l, int value)
{
    Node *n = malloc(sizeof *n);
    if (n == NULL) return false;
    n->value = value;
    n->next  = l->head;
    l->head  = n;
    if (l->tail == NULL) l->tail = n;      /* first node is also the tail */
    l->count++;
    return true;
}

static bool list_push_back(List *l, int value)
{
    Node *n = malloc(sizeof *n);
    if (n == NULL) return false;
    n->value = value;
    n->next  = NULL;
    if (l->tail != NULL) l->tail->next = n;
    else                 l->head       = n;
    l->tail = n;
    l->count++;
    return true;
}

/* The pointer-to-pointer idiom: no special case for the head. */
static bool list_remove(List *l, int value)
{
    for (Node **link = &l->head; *link != NULL; link = &(*link)->next) {
        if ((*link)->value == value) {
            Node *dead = *link;
            *link = dead->next;
            if (dead == l->tail) {
                l->tail = (link == &l->head) ? NULL : (Node *)((char *)link -
                          offsetof(Node, next));
            }
            free(dead);
            l->count--;
            return true;
        }
    }
    return false;
}

/* Reverse in place: three pointers, one pass, no allocation. */
static void list_reverse(List *l)
{
    Node *prev = NULL;
    Node *curr = l->head;
    l->tail = l->head;
    while (curr != NULL) {
        Node *next = curr->next;   /* save the rest */
        curr->next = prev;          /* flip this link */
        prev = curr;                /* advance */
        curr = next;
    }
    l->head = prev;
}

static void list_print(const List *l, const char *label)
{
    printf("  %-12s (%zu)", label, l->count);
    for (const Node *n = l->head; n != NULL; n = n->next) {
        printf(" %d", n->value);
    }
    putchar('\n');
}

/* ================= stack ================= */

typedef struct { List inner; } Stack;

static void stack_init(Stack *s)    { list_init(&s->inner); }
static void stack_destroy(Stack *s) { list_destroy(&s->inner); }
static bool stack_push(Stack *s, int v) { return list_push_front(&s->inner, v); }

static bool stack_pop(Stack *s, int *out)
{
    Node *top = s->inner.head;
    if (top == NULL) return false;
    *out = top->value;
    s->inner.head = top->next;
    if (s->inner.head == NULL) s->inner.tail = NULL;
    free(top);
    s->inner.count--;
    return true;
}

/* ================= queue ================= */

typedef struct { List inner; } Queue;

static void queue_init(Queue *q)    { list_init(&q->inner); }
static void queue_destroy(Queue *q) { list_destroy(&q->inner); }
static bool queue_enqueue(Queue *q, int v) { return list_push_back(&q->inner, v); }

static bool queue_dequeue(Queue *q, int *out)
{
    Node *front = q->inner.head;
    if (front == NULL) return false;
    *out = front->value;
    q->inner.head = front->next;
    if (q->inner.head == NULL) q->inner.tail = NULL;
    free(front);
    q->inner.count--;
    return true;
}

/* ================= measurement ================= */

#define N 200000

static double time_list_traversal(void)
{
    List l;
    list_init(&l);
    for (int i = 0; i < N; i++) list_push_front(&l, i);

    clock_t t0 = clock();
    long sum = 0;
    for (int pass = 0; pass < 20; pass++) {
        for (const Node *n = l.head; n != NULL; n = n->next) sum += n->value;
    }
    double secs = (double)(clock() - t0) / CLOCKS_PER_SEC;
    printf("    (list sum %ld)\n", sum);
    list_destroy(&l);
    return secs;
}

static double time_array_traversal(void)
{
    int *a = malloc(N * sizeof *a);
    if (a == NULL) return -1.0;
    for (int i = 0; i < N; i++) a[i] = i;

    clock_t t0 = clock();
    long sum = 0;
    for (int pass = 0; pass < 20; pass++) {
        for (int i = 0; i < N; i++) sum += a[i];
    }
    double secs = (double)(clock() - t0) / CLOCKS_PER_SEC;
    printf("    (array sum %ld)\n", sum);
    free(a);
    return secs;
}

int main(void)
{
    puts("== list operations ==");
    List l;
    list_init(&l);
    for (int i = 1; i <= 5; i++) list_push_back(&l, i * 10);
    list_print(&l, "built");

    list_push_front(&l, 5);
    list_print(&l, "push_front");

    printf("  remove 30: %s\n", list_remove(&l, 30) ? "yes" : "no");
    list_print(&l, "after");

    printf("  remove 5 (the head): %s\n", list_remove(&l, 5) ? "yes" : "no");
    list_print(&l, "after");
    puts("  no special case was needed for the head");

    printf("  remove 999: %s\n", list_remove(&l, 999) ? "yes" : "no");

    list_reverse(&l);
    list_print(&l, "reversed");

    list_destroy(&l);

    puts("\n== stack: last in, first out ==");
    Stack s;
    stack_init(&s);
    for (int i = 1; i <= 4; i++) {
        stack_push(&s, i);
        printf("  pushed %d\n", i);
    }
    int v;
    while (stack_pop(&s, &v)) printf("  popped %d\n", v);
    printf("  pop on empty: %s\n", stack_pop(&s, &v) ? "value" : "refused");
    stack_destroy(&s);

    puts("\n== queue: first in, first out ==");
    Queue q;
    queue_init(&q);
    for (int i = 1; i <= 4; i++) {
        queue_enqueue(&q, i);
        printf("  enqueued %d\n", i);
    }
    while (queue_dequeue(&q, &v)) printf("  dequeued %d\n", v);
    queue_destroy(&q);

    puts("\n== traversal: list versus array ==");
    double list_secs  = time_list_traversal();
    double array_secs = time_array_traversal();
    printf("  %d elements, 20 passes\n", N);
    printf("  linked list : %.4f s\n", list_secs);
    printf("  array       : %.4f s\n", array_secs);
    if (array_secs > 0) {
        printf("  the array is %.1fx faster for the same operation count\n",
               list_secs / array_secs);
    }
    puts("  same asymptotic complexity; the difference is cache locality");

    printf("\n  memory per element: array %zu bytes, list node %zu bytes\n",
           sizeof(int), sizeof(Node));
    puts("  plus allocator overhead of roughly 16 bytes per node");

    return EXIT_SUCCESS;
}
