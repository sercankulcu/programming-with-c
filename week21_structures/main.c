#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NAME_LEN 32

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point top_left;
    Point bottom_right;
} Rect;

typedef struct {
    char   name[NAME_LEN];
    int    score;
    int    attempts;
} Student;

/* Small and read-only: by value is fine and reads well. */
static int manhattan(Point a, Point b)
{
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return (dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy);
}

/* Read-only: const pointer. No copy, contract enforced. */
static int rect_area(const Rect *r)
{
    return (r->bottom_right.x - r->top_left.x)
         * (r->bottom_right.y - r->top_left.y);
}

/* Modifies: non-const pointer. */
static void rect_translate(Rect *r, int dx, int dy)
{
    r->top_left.x     += dx;  r->top_left.y     += dy;
    r->bottom_right.x += dx;  r->bottom_right.y += dy;
}

/* Returning by value: the caller owns the result, no lifetime question. */
static Point point_make(int x, int y)
{
    return (Point){ .x = x, .y = y };    /* compound literal — week 38 */
}

/* Structures have no ==; compare what actually matters. */
static bool point_equal(Point a, Point b)
{
    return a.x == b.x && a.y == b.y;
}

static void student_print(const Student *s)
{
    printf("  %-10s score %3d  attempts %d\n",
           s->name, s->score, s->attempts);
}

/* An array of structures, searched and aggregated. */
static const Student *find_student(const Student *list, size_t n,
                                   const char *name)
{
    for (size_t i = 0; i < n; i++) {
        if (strcmp(list[i].name, name) == 0) {
            return &list[i];             /* points into the caller's array */
        }
    }
    return NULL;
}

static bool class_average(const Student *list, size_t n, double *out)
{
    if (n == 0) {
        return false;
    }
    long total = 0;
    for (size_t i = 0; i < n; i++) {
        total += list[i].score;
    }
    *out = (double)total / (double)n;
    return true;
}

/* A self-referential type: the basis of week 32. */
typedef struct Node {
    int          value;
    struct Node *next;
} Node;

int main(void)
{
    puts("== points ==");
    Point a = { .x = 1, .y = 2 };
    Point b = point_make(4, 6);
    printf("  a = (%d,%d), b = (%d,%d), manhattan = %d\n",
           a.x, a.y, b.x, b.y, manhattan(a, b));
    printf("  a == b ? %s   (compared member by member)\n",
           point_equal(a, b) ? "yes" : "no");

    Point copy = a;
    copy.x = 99;
    printf("  after copying a and changing the copy: a.x = %d\n", a.x);
    puts("  structures assign by value — unlike arrays");

    puts("\n== nesting, and const versus mutable pointers ==");
    Rect r = {
        .top_left     = { .x = 0, .y = 0 },
        .bottom_right = { .x = 4, .y = 3 }
    };
    printf("  area = %d\n", rect_area(&r));
    rect_translate(&r, 10, 10);
    printf("  after translating: top_left = (%d,%d), area = %d\n",
           r.top_left.x, r.top_left.y, rect_area(&r));
    puts("  the area is unchanged, as it must be");

    puts("\n== an array of structures ==");
    Student class[] = {
        { .name = "Ada",    .score = 95, .attempts = 1 },
        { .name = "Dennis", .score = 88, .attempts = 2 },
        { .name = "Ken",    .score = 91, .attempts = 1 },
        { .name = "Grace",  .score = 78, .attempts = 3 }
    };
    const size_t n = sizeof class / sizeof class[0];

    for (size_t i = 0; i < n; i++) {
        student_print(&class[i]);
    }

    double average;
    if (class_average(class, n, &average)) {
        printf("  average of %zu students: %.2f\n", n, average);
    }

    const Student *found = find_student(class, n, "Ken");
    printf("  lookup \"Ken\"  : %s\n",
           found ? "found" : "not found");
    if (found != NULL) {
        student_print(found);
    }
    printf("  lookup \"Linus\": %s\n",
           find_student(class, n, "Linus") ? "found" : "not found");

    puts("\n== sizes and copying cost ==");
    printf("  sizeof(Point)   = %2zu\n", sizeof(Point));
    printf("  sizeof(Rect)    = %2zu\n", sizeof(Rect));
    printf("  sizeof(Student) = %2zu   (%d-byte name plus two ints)\n",
           sizeof(Student), NAME_LEN);
    printf("  sizeof(Student *) = %zu — which is why large structures\n",
           sizeof(Student *));
    puts("  are passed by pointer");

    puts("\n== self-referential ==");
    Node third  = { .value = 3, .next = NULL };
    Node second = { .value = 2, .next = &third };
    Node first  = { .value = 1, .next = &second };
    printf("  list: ");
    for (Node *p = &first; p != NULL; p = p->next) {
        printf("%d ", p->value);
    }
    puts("\n  a struct cannot contain itself, but it can point to itself");

    return EXIT_SUCCESS;
}
