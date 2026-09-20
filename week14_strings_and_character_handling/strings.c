#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* --- written by hand, to show what the library does --- */

static size_t my_strlen(const char *s)
{
    const char *start = s;
    while (*s != '\0') {
        s++;
    }
    return (size_t)(s - start);      /* pointer subtraction: week 15 */
}

static void my_strcpy(char *dst, const char *src)
{
    while ((*dst++ = *src++) != '\0') {
        /* copies, including the terminator, then stops */
    }
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a != '\0' && *a == *b) {
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

/* --- a safe copy, which the library does not provide --- */

static bool copy_into(char *dst, size_t dst_size, const char *src)
{
    int needed = snprintf(dst, dst_size, "%s", src);
    return needed >= 0 && (size_t)needed < dst_size;
}

static void to_upper_in_place(char *s)
{
    for (; *s != '\0'; s++) {
        *s = (char)toupper((unsigned char)*s);
    }
}

static size_t count_words(const char *s)
{
    size_t words = 0;
    bool in_word = false;
    for (; *s != '\0'; s++) {
        if (isspace((unsigned char)*s)) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            words++;
        }
    }
    return words;
}

int main(void)
{
    const char *sample = "the quick brown fox";

    puts("== hand-written versus library ==");
    printf("  my_strlen  = %zu   strlen  = %zu\n",
           my_strlen(sample), strlen(sample));

    char a[32], b[32];
    my_strcpy(a, sample);
    strcpy(b, sample);
    printf("  my_strcpy  = \"%s\"\n", a);
    printf("  strcpy     = \"%s\"\n", b);

    printf("  my_strcmp(\"abc\",\"abd\") = %d   strcmp = %d\n",
           my_strcmp("abc", "abd"), strcmp("abc", "abd"));
    printf("  equal strings give 0: %d\n", strcmp("same", "same"));

    puts("\n== strlen versus sizeof ==");
    char buffer[32] = "hello";
    printf("  strlen(buffer) = %zu   sizeof buffer = %zu\n",
           strlen(buffer), sizeof buffer);
    puts("  the difference is room for growth plus the terminator");

    puts("\n== truncation, detected ==");
    char small[8];
    if (copy_into(small, sizeof small, "short")) {
        printf("  \"short\" fitted: \"%s\"\n", small);
    }
    if (!copy_into(small, sizeof small, "a much longer string")) {
        printf("  long string truncated to \"%s\" — and we know it\n", small);
    }

    puts("\n== strncpy does not always terminate ==");
    char danger[6];
    memset(danger, 'X', sizeof danger);      /* poison, to make it visible */
    strncpy(danger, "hello world", 5);
    printf("  first five bytes: %.5s\n", danger);
    printf("  byte 5 is '%c' (%d) — not a terminator\n",
           danger[5], danger[5]);
    puts("  printing this with %s would read past the end");

    puts("\n== memmove versus memcpy ==");
    char overlap[] = "abcdef";
    memmove(overlap + 1, overlap, 5);
    printf("  memmove overlapping: %s\n", overlap);
    puts("  memcpy on the same input is undefined behavior");

    puts("\n== tokenizing modifies the buffer ==");
    char csv[] = "red,green,blue";
    printf("  before: %s\n", csv);
    for (char *tok = strtok(csv, ","); tok != NULL; tok = strtok(NULL, ",")) {
        printf("    token: %s\n", tok);
    }
    printf("  after:  %s   <-- only the first token; commas became '\\0'\n", csv);

    puts("\n== character classification ==");
    char text[32];
    strcpy(text, sample);
    printf("  words in \"%s\" = %zu\n", text, count_words(text));
    to_upper_in_place(text);
    printf("  upper-cased: %s\n", text);

    return EXIT_SUCCESS;
}
