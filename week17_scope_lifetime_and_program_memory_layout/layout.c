#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* .data — initialized, writable, whole-run lifetime */
int    global_initialized = 42;
double global_double      = 3.14;

/* .bss — zero-initialized; costs no space in the executable */
int    global_zero;
int    big_zero_array[100000];

/* .rodata — read-only */
const char *const message = "stored in read-only memory";
const int  lookup[4] = { 1, 2, 3, 4 };

/* file scope but not exported: week 28 */
static int file_private = 7;

/* A static local: block scope, static duration. */
static int call_count(void)
{
    static int calls = 0;       /* initialized once, before main */
    return ++calls;
}

/* Demonstrates that each frame is distinct and reused. */
static void frame_depth(int depth)
{
    int marker = depth;
    printf("    depth %d: &marker = %p\n", depth, (void *)&marker);
    if (depth < 3) {
        frame_depth(depth + 1);
    }
}

static void shadowing_demo(void)
{
    int file_private = 999;     /* shadows the file-scope one */
    printf("  inside the function, file_private = %d\n", file_private);
    printf("  (the file-scope one is still 7, but unreachable by name here)\n");
}

int main(void)
{
    int    local          = 1;
    char   local_array[64] = "on the stack";
    void  *heap_block      = malloc(1024);

    if (heap_block == NULL) {
        fprintf(stderr, "allocation failed\n");
        return EXIT_FAILURE;
    }

    puts("== where things live (addresses vary between runs) ==\n");
    printf("  %-26s %p\n", ".text  (code: main)",      (void *)(void *)main);
    printf("  %-26s %p\n", ".rodata (string literal)", (void *)message);
    printf("  %-26s %p\n", ".rodata (const array)",    (void *)lookup);
    printf("  %-26s %p\n", ".data  (global = 42)",     (void *)&global_initialized);
    printf("  %-26s %p\n", ".data  (static file var)", (void *)&file_private);
    printf("  %-26s %p\n", ".bss   (global zero)",     (void *)&global_zero);
    printf("  %-26s %p\n", ".bss   (big zero array)",  (void *)big_zero_array);
    printf("  %-26s %p\n", "heap   (malloc 1024)",     heap_block);
    printf("  %-26s %p\n", "stack  (local int)",       (void *)&local);
    printf("  %-26s %p\n", "stack  (local array)",     (void *)local_array);

    puts("\n  read the addresses: code and constants lowest, then the");
    puts("  writable statics, then the heap, with the stack far above");

    puts("\n== automatic versus static duration ==");
    printf("  call_count() -> %d\n", call_count());
    printf("  call_count() -> %d\n", call_count());
    printf("  call_count() -> %d   (the value persisted between calls)\n",
           call_count());

    puts("\n== zero-initialization ==");
    printf("  global_zero (static, no initializer) = %d  -- guaranteed 0\n",
           global_zero);
    puts("  an uninitialized LOCAL would hold garbage instead");

    puts("\n== stack frames ==");
    printf("  main's local is at      %p\n", (void *)&local);
    frame_depth(1);
    puts("  each frame sits at a lower address; they are popped on return");

    puts("\n== scope and shadowing ==");
    printf("  at file scope, file_private = %d\n", file_private);
    shadowing_demo();

    puts("\n== read-only memory ==");
    printf("  message = \"%s\"\n", message);
    puts("  writing through it would be refused by the hardware");

    free(heap_block);
    return EXIT_SUCCESS;
}
