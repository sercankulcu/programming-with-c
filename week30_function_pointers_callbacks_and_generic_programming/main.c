#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>

/* ================= generic vector ================= */

typedef struct {
    void   *data;
    size_t  count;
    size_t  capacity;
    size_t  element_size;
} Vector;

static bool vector_init(Vector *v, size_t element_size, size_t capacity)
{
    if (capacity == 0) capacity = 4;
    v->data = malloc(capacity * element_size);
    if (v->data == NULL) return false;
    v->count = 0;
    v->capacity = capacity;
    v->element_size = element_size;
    return true;
}

static void vector_destroy(Vector *v)
{
    free(v->data);
    v->data = NULL;
    v->count = v->capacity = 0;
}

static void *vector_at(const Vector *v, size_t i)
{
    return (char *)v->data + i * v->element_size;   /* byte arithmetic */
}

static bool vector_push(Vector *v, const void *element)
{
    if (v->count == v->capacity) {
        size_t bigger = v->capacity * 2;
        void *moved = realloc(v->data, bigger * v->element_size);
        if (moved == NULL) return false;
        v->data = moved;
        v->capacity = bigger;
    }
    memcpy(vector_at(v, v->count), element, v->element_size);
    v->count++;
    return true;
}

/* A callback applied to every element: the visitor pattern in C. */
static void vector_each(const Vector *v, void (*visit)(const void *, void *),
                        void *context)
{
    for (size_t i = 0; i < v->count; i++) {
        visit(vector_at(v, i), context);
    }
}

static void vector_sort(Vector *v, int (*cmp)(const void *, const void *))
{
    qsort(v->data, v->count, v->element_size, cmp);
}

/* ================= comparators ================= */

typedef struct { char name[16]; int score; } Student;

/* Never x - y: it overflows. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

static int cmp_double(const void *a, const void *b)
{
    double x = *(const double *)a, y = *(const double *)b;
    return (x > y) - (x < y);
}

static int cmp_student_score(const void *a, const void *b)
{
    const Student *x = a, *y = b;
    return (y->score > x->score) - (y->score < x->score);   /* descending */
}

static int cmp_student_name(const void *a, const void *b)
{
    return strcmp(((const Student *)a)->name, ((const Student *)b)->name);
}

/* ================= visitors ================= */

static void print_int(const void *e, void *ctx)
{
    (void)ctx;
    printf("%d ", *(const int *)e);
}

static void sum_int(const void *e, void *ctx)
{
    *(long *)ctx += *(const int *)e;
}

static void print_student(const void *e, void *ctx)
{
    (void)ctx;
    const Student *s = e;
    printf("    %-10s %3d\n", s->name, s->score);
}

/* ================= dispatch table ================= */

static int op_add(int a, int b) { return a + b; }
static int op_sub(int a, int b) { return a - b; }
static int op_mul(int a, int b) { return a * b; }

typedef struct {
    const char *name;
    int (*fn)(int, int);
    const char *help;
} Command;

static const Command commands[] = {
    { "add", op_add, "a + b" },
    { "sub", op_sub, "a - b" },
    { "mul", op_mul, "a * b" }
};
#define COMMAND_COUNT (sizeof commands / sizeof commands[0])

/* ================= polymorphism ================= */

typedef struct Shape {
    const char *name;
    double (*area)(const struct Shape *);
} Shape;

typedef struct { Shape base; double radius; }        Circle;
typedef struct { Shape base; double w, h; }          Rect;

static double circle_area(const Shape *s)
{
    return 3.14159265358979 * ((const Circle *)s)->radius
                            * ((const Circle *)s)->radius;
}
static double rect_area(const Shape *s)
{
    const Rect *r = (const Rect *)s;
    return r->w * r->h;
}

/* ================= variadic ================= */

static int sum_all(int count, ...)
{
    va_list args;
    va_start(args, count);
    int total = 0;
    for (int i = 0; i < count; i++) {
        total += va_arg(args, int);
    }
    va_end(args);
    return total;
}

/* A sentinel-terminated variadic function: no count needed. */
static void print_all(const char *first, ...)
{
    va_list args;
    va_start(args, first);
    for (const char *s = first; s != NULL; s = va_arg(args, const char *)) {
        printf("[%s] ", s);
    }
    va_end(args);
    putchar('\n');
}

static void log_message(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "log: ");
    vfprintf(stderr, fmt, args);     /* the v-variant takes a va_list */
    va_end(args);
}

/* ================= _Generic ================= */

#define type_name(x) _Generic((x),   \
    int:          "int",             \
    long:         "long",            \
    double:       "double",          \
    char *:       "char *",          \
    const char *: "const char *",    \
    default:      "other")

int main(void)
{
    puts("== one implementation, three element types ==");

    Vector ints;
    vector_init(&ints, sizeof(int), 4);
    int sample[] = { 42, 7, 99, 13, 5, 77 };
    for (size_t i = 0; i < 6; i++) vector_push(&ints, &sample[i]);

    printf("  unsorted: ");  vector_each(&ints, print_int, NULL);
    vector_sort(&ints, cmp_int);
    printf("\n  sorted:   ");  vector_each(&ints, print_int, NULL);

    long total = 0;
    vector_each(&ints, sum_int, &total);
    printf("\n  sum via a visitor with context: %ld\n", total);

    int key = 99;
    int *found = bsearch(&key, ints.data, ints.count, sizeof(int), cmp_int);
    printf("  bsearch 99: %s\n", found ? "found" : "not found");

    Vector doubles;
    vector_init(&doubles, sizeof(double), 4);
    double d[] = { 2.5, 1.0, 3.75 };
    for (size_t i = 0; i < 3; i++) vector_push(&doubles, &d[i]);
    vector_sort(&doubles, cmp_double);
    printf("  doubles sorted: %g %g %g\n",
           *(double *)vector_at(&doubles, 0),
           *(double *)vector_at(&doubles, 1),
           *(double *)vector_at(&doubles, 2));

    Vector students;
    vector_init(&students, sizeof(Student), 4);
    Student roster[] = {
        { "Ada", 95 }, { "Dennis", 88 }, { "Ken", 91 }, { "Grace", 78 }
    };
    for (size_t i = 0; i < 4; i++) vector_push(&students, &roster[i]);

    puts("  students by score:");
    vector_sort(&students, cmp_student_score);
    vector_each(&students, print_student, NULL);
    puts("  students by name:");
    vector_sort(&students, cmp_student_name);
    vector_each(&students, print_student, NULL);
    puts("  the same sort and the same container, three element types");

    puts("\n== dispatch table ==");
    for (size_t i = 0; i < COMMAND_COUNT; i++) {
        printf("  %-4s (%s): %d\n", commands[i].name, commands[i].help,
               commands[i].fn(10, 3));
    }

    puts("\n== polymorphism through a function pointer ==");
    Circle c = { { "circle", circle_area }, 2.0 };
    Rect   r = { { "rect",   rect_area   }, 3.0, 4.0 };
    Shape *shapes[] = { (Shape *)&c, (Shape *)&r };
    for (size_t i = 0; i < 2; i++) {
        printf("  %-7s area = %.4f\n", shapes[i]->name,
               shapes[i]->area(shapes[i]));
    }

    puts("\n== variadic functions ==");
    printf("  sum_all(3, 1,2,3)       = %d\n", sum_all(3, 1, 2, 3));
    printf("  sum_all(5, 10,20,30,40,50) = %d\n",
           sum_all(5, 10, 20, 30, 40, 50));
    printf("  sentinel-terminated: ");
    print_all("alpha", "beta", "gamma", NULL);
    log_message("a wrapped printf, value = %d\n", 42);

    puts("\n== _Generic ==");
    int i_val = 1; double d_val = 1.0; const char *s_val = "x";
    printf("  1     is %s\n", type_name(i_val));
    printf("  1.0   is %s\n", type_name(d_val));
    printf("  \"x\"   is %s\n", type_name(s_val));

    vector_destroy(&ints);
    vector_destroy(&doubles);
    vector_destroy(&students);
    return EXIT_SUCCESS;
}
