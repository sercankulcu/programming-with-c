#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>

#define LINE_MAX_LEN 256

/* ---------- the contract ---------- */

typedef enum {
    FC_OK = 0,
    FC_ERRNO,
    FC_BAD_FORMAT,
    FC_TOO_LONG,
    FC_NO_MEMORY
} FcStatus;

static const char *fc_status_name(FcStatus s)
{
    switch (s) {                       /* no default: see week 22 */
    case FC_OK:         return "ok";
    case FC_ERRNO:      return "system error";
    case FC_BAD_FORMAT: return "malformed input";
    case FC_TOO_LONG:   return "line too long";
    case FC_NO_MEMORY:  return "out of memory";
    }
    return "unknown";
}

/* ---------- the function with four resources ---------- */

/* Copies in_path to out_path, doubling every number it finds.
   Returns FC_OK on success. On failure, out_path may exist but
   its contents are unspecified; nothing the caller owns is freed. */
static FcStatus double_numbers(const char *in_path, const char *out_path,
                               long *lines_out)
{
    assert(in_path  != NULL);          /* programmer error if violated */
    assert(out_path != NULL);
    assert(lines_out != NULL);

    FcStatus  status = FC_ERRNO;       /* pessimistic default */
    FILE     *in     = NULL;
    FILE     *out    = NULL;
    char     *buffer = NULL;
    long      lines  = 0;

    in = fopen(in_path, "r");
    if (in == NULL) {
        goto cleanup;
    }

    out = fopen(out_path, "w");
    if (out == NULL) {
        goto cleanup;
    }

    buffer = malloc(LINE_MAX_LEN);
    if (buffer == NULL) {
        status = FC_NO_MEMORY;
        goto cleanup;
    }

    while (fgets(buffer, LINE_MAX_LEN, in) != NULL) {
        lines++;

        if (strchr(buffer, '\n') == NULL && !feof(in)) {
            status = FC_TOO_LONG;
            goto cleanup;
        }
        buffer[strcspn(buffer, "\n")] = '\0';

        if (buffer[0] == '\0') {
            continue;
        }

        errno = 0;
        char *end;
        long value = strtol(buffer, &end, 10);
        if (end == buffer || *end != '\0' || errno == ERANGE) {
            status = FC_BAD_FORMAT;
            goto cleanup;
        }

        if (fprintf(out, "%ld\n", value * 2) < 0) {
            status = FC_ERRNO;
            goto cleanup;
        }
    }

    if (ferror(in)) {
        status = FC_ERRNO;
        goto cleanup;
    }

    *lines_out = lines;                /* written only on success */
    status = FC_OK;

cleanup:
    free(buffer);                      /* free(NULL) is safe */
    if (out != NULL && fclose(out) != 0 && status == FC_OK) {
        status = FC_ERRNO;             /* the flush failed */
    }
    if (in != NULL) {
        fclose(in);
    }
    return status;
}

/* ---------- helper to build test inputs ---------- */

static bool make_file(const char *path, const char *contents)
{
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        return false;
    }
    fputs(contents, f);
    return fclose(f) == 0;
}

static void try_case(const char *label, const char *contents)
{
    const char *in  = "fc_in.txt";
    const char *out = "fc_out.txt";

    if (contents != NULL && !make_file(in, contents)) {
        perror("creating input");
        return;
    }

    long lines = -1;
    FcStatus s = double_numbers(contents == NULL ? "does_not_exist.txt" : in,
                                out, &lines);

    printf("  %-22s -> %-16s", label, fc_status_name(s));
    if (s == FC_OK) {
        printf("(%ld lines)\n", lines);
    } else if (s == FC_ERRNO) {
        printf("(%s)\n", strerror(errno));
    } else {
        printf("(lines_out untouched: %ld)\n", lines);
    }

    remove(in);
    remove(out);
}

int main(void)
{
    puts("== one contract, every failure path exercised ==");
    try_case("normal input",     "1\n2\n3\n");
    try_case("blank lines",      "1\n\n2\n");
    try_case("not a number",     "1\nabc\n");
    try_case("trailing rubbish", "1\n12xyz\n");
    try_case("out of range",     "99999999999999999999\n");
    try_case("missing file",     NULL);

    puts("\n== errno must be saved before reporting ==");
    errno = 0;
    FILE *f = fopen("definitely_not_here.txt", "r");
    if (f == NULL) {
        int saved = errno;
        fprintf(stderr, "  captured errno = %d\n", saved);
        fprintf(stderr, "  message: %s\n", strerror(saved));
        printf("  errno after the fprintf calls is now %d\n", errno);
        puts("  which is why the value is captured immediately");
    }

    puts("\n== assert is for programmer error, not user error ==");
    puts("  double_numbers asserts its pointers are non-null:");
    puts("  that is the caller's contract, not a runtime condition.");
    puts("  A missing file is handled; a null path is a bug.");

    puts("\n== static_assert: checked at compile time ==");
    static_assert(sizeof(long) >= 4, "long must be at least 32 bits");
    puts("  sizeof(long) >= 4 verified before the program ran");

    return EXIT_SUCCESS;
}
