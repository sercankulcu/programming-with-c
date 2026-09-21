#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- enumeration with a checkable switch ---------- */

typedef enum {
    TOKEN_NUMBER,
    TOKEN_WORD,
    TOKEN_SYMBOL
} TokenKind;

/* No default label: adding a TokenKind makes the compiler
   point at this function. That is the whole reason for the enum. */
static const char *kind_name(TokenKind k)
{
    switch (k) {
    case TOKEN_NUMBER: return "number";
    case TOKEN_WORD:   return "word";
    case TOKEN_SYMBOL: return "symbol";
    }
    return "unknown";          /* reached only for an out-of-range value */
}

/* ---------- tagged union ---------- */

typedef struct {
    TokenKind kind;
    union {
        double as_number;
        char   as_word[24];
        char   as_symbol;
    } data;
} Token;

static Token token_number(double v)
{
    Token t = { .kind = TOKEN_NUMBER };
    t.data.as_number = v;
    return t;
}

static Token token_word(const char *w)
{
    Token t = { .kind = TOKEN_WORD };
    snprintf(t.data.as_word, sizeof t.data.as_word, "%s", w);
    return t;
}

static Token token_symbol(char c)
{
    Token t = { .kind = TOKEN_SYMBOL };
    t.data.as_symbol = c;
    return t;
}

static void token_print(const Token *t)
{
    printf("  %-7s ", kind_name(t->kind));
    switch (t->kind) {
    case TOKEN_NUMBER: printf("%g\n",  t->data.as_number); break;
    case TOKEN_WORD:   printf("\"%s\"\n", t->data.as_word); break;
    case TOKEN_SYMBOL: printf("'%c'\n", t->data.as_symbol); break;
    }
}

/* ---------- named bit flags ---------- */

typedef enum {
    PERM_READ  = 1 << 0,
    PERM_WRITE = 1 << 1,
    PERM_EXEC  = 1 << 2
} Permission;

static void print_permissions(unsigned flags)
{
    printf("  %c%c%c  (0x%02X)\n",
           (flags & PERM_READ)  ? 'r' : '-',
           (flags & PERM_WRITE) ? 'w' : '-',
           (flags & PERM_EXEC)  ? 'x' : '-',
           flags);
}

/* ---------- padding ---------- */

struct Wasteful { char a; double b; char c; };
struct Tight    { double b; char a; char c; };

/* ---------- type punning, the legitimate use of a bare union ---------- */

union FloatBits {
    float    value;
    uint32_t bits;
};

int main(void)
{
    puts("== tagged union ==");
    Token tokens[] = {
        token_number(3.14),
        token_word("hello"),
        token_symbol('+'),
        token_number(42)
    };
    const size_t n = sizeof tokens / sizeof tokens[0];
    for (size_t i = 0; i < n; i++) {
        token_print(&tokens[i]);
    }
    printf("  sizeof(Token) = %zu: a tag plus the largest member\n",
           sizeof(Token));

    puts("\n== what a bare union looks like when you lose track ==");
    union { int as_int; float as_float; } raw;
    raw.as_int = 1065353216;
    printf("  stored as int   : %d\n", raw.as_int);
    printf("  read as float   : %g   <-- same bytes, different meaning\n",
           (double)raw.as_float);
    puts("  nothing in the union records which member is live");

    puts("\n== bit flags from an enumeration ==");
    unsigned p = PERM_READ | PERM_WRITE;
    print_permissions(p);
    p |= PERM_EXEC;
    print_permissions(p);
    p &= ~(unsigned)PERM_WRITE;
    print_permissions(p);

    puts("\n== padding: same members, different order ==");
    printf("  sizeof(struct Wasteful) = %zu\n", sizeof(struct Wasteful));
    printf("    a at %zu, b at %zu, c at %zu\n",
           offsetof(struct Wasteful, a),
           offsetof(struct Wasteful, b),
           offsetof(struct Wasteful, c));
    printf("  sizeof(struct Tight)    = %zu\n", sizeof(struct Tight));
    printf("    b at %zu, a at %zu, c at %zu\n",
           offsetof(struct Tight, b),
           offsetof(struct Tight, a),
           offsetof(struct Tight, c));
    printf("  the members total %zu bytes; the rest is padding\n",
           sizeof(char) * 2 + sizeof(double));
    printf("  one million records: %zu MB versus %zu MB\n",
           sizeof(struct Wasteful) * 1000000 / 1048576,
           sizeof(struct Tight)    * 1000000 / 1048576);

    puts("\n== why memcmp fails on structures ==");
    struct Wasteful p1, p2;
    memset(&p1, 0x00, sizeof p1);
    memset(&p2, 0xFF, sizeof p2);
    p1.a = 'A'; p1.b = 1.5; p1.c = 'C';
    p2.a = 'A'; p2.b = 1.5; p2.c = 'C';
    printf("  all members equal : %s\n",
           (p1.a == p2.a && p1.b == p2.b && p1.c == p2.c) ? "yes" : "no");
    printf("  memcmp says equal : %s\n",
           memcmp(&p1, &p2, sizeof p1) == 0 ? "yes" : "no");
    puts("  the padding bytes differ, and memcmp compares them too");

    puts("\n== type punning through a union ==");
    union FloatBits fb;
    fb.value = 1.0f;
    printf("  1.0f has the bit pattern 0x%08X\n", fb.bits);
    fb.bits = 0x40490FDB;
    printf("  0x40490FDB read as a float is %g\n", (double)fb.value);

    return EXIT_SUCCESS;
}
