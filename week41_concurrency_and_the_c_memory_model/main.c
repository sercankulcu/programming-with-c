#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define QUEUE_CAPACITY 8
#define PRODUCERS      3
#define CONSUMERS      2
#define PER_PRODUCER   2000

/* ---------- 1. the race, demonstrated ---------- */

static long   racy_counter   = 0;
static atomic_long atomic_counter = 0;
static long   guarded_counter = 0;
static pthread_mutex_t guard = PTHREAD_MUTEX_INITIALIZER;

#define BUMPS 200000

static void *bump_all(void *arg)
{
    (void)arg;
    for (int i = 0; i < BUMPS; i++) {
        racy_counter++;                               /* DATA RACE */
        atomic_fetch_add(&atomic_counter, 1);         /* atomic     */
        pthread_mutex_lock(&guard);
        guarded_counter++;                            /* protected  */
        pthread_mutex_unlock(&guard);
    }
    return NULL;
}

/* ---------- 2. a bounded blocking queue ---------- */

typedef struct {
    int             items[QUEUE_CAPACITY];
    size_t          head, tail, count;
    bool            closed;
    pthread_mutex_t lock;
    pthread_cond_t  not_empty;
    pthread_cond_t  not_full;
} Queue;

static void queue_init(Queue *q)
{
    q->head = q->tail = q->count = 0;
    q->closed = false;
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);
}

static void queue_destroy(Queue *q)
{
    pthread_mutex_destroy(&q->lock);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
}

static void queue_push(Queue *q, int value)
{
    pthread_mutex_lock(&q->lock);
    while (q->count == QUEUE_CAPACITY && !q->closed) {
        pthread_cond_wait(&q->not_full, &q->lock);    /* while, not if */
    }
    if (!q->closed) {
        q->items[q->tail] = value;
        q->tail = (q->tail + 1) % QUEUE_CAPACITY;
        q->count++;
        pthread_cond_signal(&q->not_empty);
    }
    pthread_mutex_unlock(&q->lock);
}

static bool queue_pop(Queue *q, int *out)
{
    pthread_mutex_lock(&q->lock);
    while (q->count == 0 && !q->closed) {
        pthread_cond_wait(&q->not_empty, &q->lock);
    }
    if (q->count == 0) {                              /* closed and drained */
        pthread_mutex_unlock(&q->lock);
        return false;
    }
    *out = q->items[q->head];
    q->head = (q->head + 1) % QUEUE_CAPACITY;
    q->count--;
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->lock);
    return true;
}

static void queue_close(Queue *q)
{
    pthread_mutex_lock(&q->lock);
    q->closed = true;
    pthread_cond_broadcast(&q->not_empty);            /* wake everyone */
    pthread_cond_broadcast(&q->not_full);
    pthread_mutex_unlock(&q->lock);
}

/* ---------- 3. the workers ---------- */

typedef struct { Queue *q; int id; } Worker;

static atomic_long produced = 0;
static atomic_long consumed = 0;
static atomic_long checksum = 0;

_Thread_local long my_items = 0;                      /* private per thread */

static void *producer(void *arg)
{
    Worker *w = arg;                                  /* its own object */
    for (int i = 0; i < PER_PRODUCER; i++) {
        int value = w->id * 1000000 + i;
        queue_push(w->q, value);
        atomic_fetch_add(&produced, 1);
        my_items++;
    }
    printf("  producer %d finished, %ld items (thread-local count)\n",
           w->id, my_items);
    return NULL;
}

static void *consumer(void *arg)
{
    Worker *w = arg;
    int value;
    while (queue_pop(w->q, &value)) {
        atomic_fetch_add(&consumed, 1);
        atomic_fetch_add(&checksum, value % 97);
        my_items++;
    }
    printf("  consumer %d finished, %ld items\n", w->id, my_items);
    return NULL;
}

int main(void)
{
    puts("== 1. three counters, one loop, four threads ==");
    pthread_t bumpers[4];
    for (int i = 0; i < 4; i++) pthread_create(&bumpers[i], NULL, bump_all, NULL);
    for (int i = 0; i < 4; i++) pthread_join(bumpers[i], NULL);

    long expected = 4L * BUMPS;
    printf("  expected          : %ld\n", expected);
    printf("  racy (unprotected): %ld  %s\n", racy_counter,
           racy_counter == expected ? "(lucky this run)" : "<-- updates lost");
    printf("  atomic            : %ld\n", atomic_load(&atomic_counter));
    printf("  mutex-protected   : %ld\n", guarded_counter);
    puts("  the racy result varies between runs; that is what UB looks like");

    puts("\n== 2. producer-consumer with a bounded queue ==");
    Queue q;
    queue_init(&q);

    pthread_t pt[PRODUCERS], ct[CONSUMERS];
    Worker    pw[PRODUCERS], cw[CONSUMERS];

    struct timespec t0;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    for (int i = 0; i < CONSUMERS; i++) {
        cw[i] = (Worker){ .q = &q, .id = i };         /* its own object */
        pthread_create(&ct[i], NULL, consumer, &cw[i]);
    }
    for (int i = 0; i < PRODUCERS; i++) {
        pw[i] = (Worker){ .q = &q, .id = i };
        pthread_create(&pt[i], NULL, producer, &pw[i]);
    }

    for (int i = 0; i < PRODUCERS; i++) pthread_join(pt[i], NULL);
    queue_close(&q);                                   /* now the consumers end */
    for (int i = 0; i < CONSUMERS; i++) pthread_join(ct[i], NULL);

    struct timespec t1;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double secs = (double)(t1.tv_sec - t0.tv_sec)
                + (double)(t1.tv_nsec - t0.tv_nsec) / 1e9;

    printf("  produced %ld, consumed %ld, checksum %ld, %.3f s\n",
           atomic_load(&produced), atomic_load(&consumed),
           atomic_load(&checksum), secs);
    printf("  queue capacity was %d, so producers blocked whenever\n",
           QUEUE_CAPACITY);
    puts("  consumers fell behind — that is back-pressure, for free");

    queue_destroy(&q);

    puts("\n== 3. reentrancy ==");
    time_t now = time(NULL);
    struct tm safe;
    localtime_r(&now, &safe);                          /* caller's buffer */
    char stamp[64];
    strftime(stamp, sizeof stamp, "%H:%M:%S", &safe);
    printf("  localtime_r : %s\n", stamp);
    puts("  localtime() returns a pointer to a shared static buffer;");
    puts("  two threads calling it would overwrite each other's result");

    char text[] = "alpha,beta,gamma";
    char *save = NULL;
    printf("  strtok_r    :");
    for (char *tok = strtok_r(text, ",", &save); tok != NULL;
         tok = strtok_r(NULL, ",", &save)) {
        printf(" [%s]", tok);
    }
    puts("\n  the save pointer replaces strtok's hidden static state");

    return EXIT_SUCCESS;
}
