#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

/* ---------- named flags ---------- */

typedef enum {
    PERM_READ    = 1u << 0,
    PERM_WRITE   = 1u << 1,
    PERM_EXEC    = 1u << 2,
    PERM_DELETE  = 1u << 3,
    PERM_ALL     = 0x0Fu
} Permission;

static void print_bits(uint32_t value, int width)
{
    for (int bit = width - 1; bit >= 0; bit--) {
        putchar((value >> bit) & 1u ? '1' : '0');
        if (bit % 4 == 0 && bit != 0) putchar(' ');
    }
}

static void show_permissions(const char *label, unsigned flags)
{
    printf("  %-14s %c%c%c%c  ", label,
           (flags & PERM_READ)   ? 'r' : '-',
           (flags & PERM_WRITE)  ? 'w' : '-',
           (flags & PERM_EXEC)   ? 'x' : '-',
           (flags & PERM_DELETE) ? 'd' : '-');
    print_bits(flags, 8);
    printf("  (0x%02X)\n", flags);
}

/* ---------- packing ---------- */

static uint32_t rgb_pack(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}

static void rgb_unpack(uint32_t rgb, uint8_t *r, uint8_t *g, uint8_t *b)
{
    *r = (uint8_t)((rgb >> 16) & 0xFFu);
    *g = (uint8_t)((rgb >>  8) & 0xFFu);
    *b = (uint8_t)( rgb        & 0xFFu);
}

/* ---------- counting set bits, three ways ---------- */

static int popcount_naive(uint32_t v)
{
    int count = 0;
    for (int i = 0; i < 32; i++) {
        count += (int)((v >> i) & 1u);
    }
    return count;                          /* always 32 iterations */
}

/* Kernighan's method: v & (v-1) clears the lowest set bit,
   so the loop runs once per SET bit rather than once per bit. */
static int popcount_kernighan(uint32_t v)
{
    int count = 0;
    while (v != 0) {
        v &= v - 1;
        count++;
    }
    return count;
}

/* ---------- a simple checksum ---------- */

static uint32_t checksum(const uint8_t *data, size_t n)
{
    uint32_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum ^= data[i];
        sum = (sum << 1) | (sum >> 31);    /* rotate left by one */
    }
    return sum;
}

/* ---------- bit-fields, for internal use only ---------- */

struct Packed {
    unsigned read  : 1;
    unsigned write : 1;
    unsigned level : 3;
};

int main(void)
{
    puts("== the four idioms ==");
    unsigned flags = 0;
    show_permissions("start", flags);

    flags |= PERM_READ;                   show_permissions("set read", flags);
    flags |= PERM_WRITE | PERM_EXEC;      show_permissions("set w+x", flags);
    flags &= ~(unsigned)PERM_WRITE;       show_permissions("clear write", flags);
    flags ^= PERM_EXEC;                   show_permissions("toggle exec", flags);
    flags |= PERM_ALL;                    show_permissions("set all", flags);

    puts("\n== any versus all ==");
    unsigned rw = PERM_READ | PERM_WRITE;
    unsigned only_read = PERM_READ;
    printf("  only_read has ANY of r|w : %s\n",
           (only_read & rw) ? "yes" : "no");
    printf("  only_read has ALL of r|w : %s\n",
           ((only_read & rw) == rw) ? "yes" : "no");

    puts("\n== the precedence trap ==");
    unsigned f = 0x0C;
    printf("  f & PERM_EXEC == 0    gives %u   <-- parses as f & (EXEC==0)\n",
           f & PERM_EXEC == 0);
    printf("  (f & PERM_EXEC) == 0  gives %d   <-- what you meant\n",
           (f & PERM_EXEC) == 0);

    puts("\n== packing and unpacking ==");
    uint32_t colour = rgb_pack(255, 128, 64);
    printf("  rgb(255,128,64) = 0x%06X  ", colour);
    print_bits(colour, 24);
    putchar('\n');
    uint8_t r, g, b;
    rgb_unpack(colour, &r, &g, &b);
    printf("  unpacked        = (%u, %u, %u)\n", r, g, b);
    printf("  without the mask, (colour >> 8) = 0x%X  <-- red bits still there\n",
           colour >> 8);

    puts("\n== shifting ==");
    uint32_t v = 0x0000000Fu;
    printf("  0x0F << 4  = 0x%08X   (multiply by 16)\n", v << 4);
    printf("  0xF0 >> 4  = 0x%08X   (divide by 16)\n", 0xF0u >> 4);
    int negative = -16;
    printf("  -16 >> 2   = %d   (arithmetic shift here, but not guaranteed)\n",
           negative >> 2);
    puts("  do bit work on unsigned types and the question does not arise");

    puts("\n== counting set bits ==");
    uint32_t samples[] = { 0x00000000u, 0x00000001u, 0x0000FFFFu, 0xFFFFFFFFu };
    for (size_t i = 0; i < 4; i++) {
        printf("  0x%08X : naive %2d, kernighan %2d\n", samples[i],
               popcount_naive(samples[i]),
               popcount_kernighan(samples[i]));
    }

    clock_t t0 = clock();
    long total = 0;
    for (uint32_t i = 0; i < 3000000u; i++) total += popcount_naive(i);
    double naive_time = (double)(clock() - t0) / CLOCKS_PER_SEC;

    t0 = clock();
    total = 0;
    for (uint32_t i = 0; i < 3000000u; i++) total += popcount_kernighan(i);
    double kern_time = (double)(clock() - t0) / CLOCKS_PER_SEC;

    printf("  3M values: naive %.3fs, Kernighan %.3fs\n",
           naive_time, kern_time);
    puts("  v &= v-1 clears the lowest set bit, so the loop runs once per 1");

    puts("\n== a checksum ==");
    const char *text = "the quick brown fox";
    printf("  checksum(\"%s\") = 0x%08X\n",
           text, checksum((const uint8_t *)text, 19));
    printf("  checksum(\"the quick brown fpx\") = 0x%08X  <-- one byte changed\n",
           checksum((const uint8_t *)"the quick brown fpx", 19));

    puts("\n== bit-fields: convenient, but not portable ==");
    struct Packed p = { .read = 1, .write = 0, .level = 5 };
    printf("  read=%u write=%u level=%u, sizeof = %zu\n",
           p.read, p.write, p.level, sizeof p);
    puts("  the standard does not fix the order or the padding,");
    puts("  so never use a bit-field struct to describe a file or packet format");

    return EXIT_SUCCESS;
}
