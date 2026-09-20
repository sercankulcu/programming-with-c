#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define NAME_LEN 32
#define LINE_LEN 256

typedef struct {
    int    id;
    char   name[NAME_LEN];
    double score;
} Record;

static const char *TEXT_FILE   = "records.csv";
static const char *BINARY_FILE = "records.dat";

/* ---------- create some input ---------- */

static bool write_sample_text(void)
{
    FILE *f = fopen(TEXT_FILE, "w");
    if (f == NULL) {
        perror(TEXT_FILE);
        return false;
    }

    fputs("# id,name,score\n", f);
    fprintf(f, "1,Ada,95.5\n");
    fprintf(f, "2,Dennis,88.0\n");
    fprintf(f, "3,Ken,91.25\n");
    fprintf(f, "4,Grace,78.5\n");
    fputs("bad line with no commas\n", f);
    fprintf(f, "5,Linus,not-a-number\n");

    if (ferror(f)) {
        perror("writing " );
        fclose(f);
        return false;
    }
    if (fclose(f) != 0) {            /* the flush can fail here */
        perror("closing " );
        return false;
    }
    return true;
}

/* ---------- read text, one line at a time ---------- */

static bool parse_line(const char *line, Record *out)
{
    char name[NAME_LEN];
    int  id;
    double score;

    /* %31[^,] reads up to 31 characters that are not a comma */
    if (sscanf(line, "%d,%31[^,],%lf", &id, name, &score) != 3) {
        return false;
    }
    out->id = id;
    snprintf(out->name, sizeof out->name, "%s", name);
    out->score = score;
    return true;
}

static size_t read_text(Record *records, size_t capacity)
{
    FILE *f = fopen(TEXT_FILE, "r");
    if (f == NULL) {
        perror(TEXT_FILE);
        return 0;
    }

    char line[LINE_LEN];
    size_t count = 0;
    long line_number = 0;

    while (count < capacity && fgets(line, sizeof line, f) != NULL) {
        line_number++;

        if (strchr(line, '\n') == NULL && !feof(f)) {
            fprintf(stderr, "  line %ld too long, skipping\n", line_number);
            int c;
            while ((c = fgetc(f)) != '\n' && c != EOF) { }
            continue;
        }
        line[strcspn(line, "\n")] = '\0';

        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }
        if (!parse_line(line, &records[count])) {
            fprintf(stderr, "  line %ld rejected: \"%s\"\n", line_number, line);
            continue;
        }
        count++;
    }

    /* Only NOW ask which condition ended the loop. */
    if (ferror(f)) {
        perror("reading " );
    }
    fclose(f);
    return count;
}

/* ---------- write fixed-size binary records ---------- */

static bool write_binary(const Record *records, size_t count)
{
    FILE *f = fopen(BINARY_FILE, "wb");      /* b matters on Windows */
    if (f == NULL) {
        perror(BINARY_FILE);
        return false;
    }
    size_t written = fwrite(records, sizeof *records, count, f);
    if (written != count) {
        fprintf(stderr, "  wrote only %zu of %zu records\n", written, count);
        fclose(f);
        return false;
    }
    return fclose(f) == 0;
}

/* ---------- seek directly to record n ---------- */

static bool read_record_at(size_t index, Record *out)
{
    FILE *f = fopen(BINARY_FILE, "rb");
    if (f == NULL) {
        perror(BINARY_FILE);
        return false;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return false;
    }
    long size = ftell(f);
    long total = size / (long)sizeof(Record);

    if ((long)index >= total) {
        fprintf(stderr, "  index %zu out of range (%ld records)\n",
                index, total);
        fclose(f);
        return false;
    }

    if (fseek(f, (long)(index * sizeof(Record)), SEEK_SET) != 0) {
        fclose(f);
        return false;
    }
    bool ok = fread(out, sizeof *out, 1, f) == 1;
    fclose(f);
    return ok;
}

static void print_record(const Record *r)
{
    printf("  id=%-3d %-10s %6.2f\n", r->id, r->name, r->score);
}

int main(void)
{
    puts("== writing sample text ==");
    if (!write_sample_text()) {
        return EXIT_FAILURE;
    }
    puts("  wrote " );

    puts("\n== reading it back, rejecting bad lines ==");
    Record records[16];
    size_t count = read_text(records, 16);
    printf("  accepted %zu records:\n", count);
    for (size_t i = 0; i < count; i++) {
        print_record(&records[i]);
    }

    puts("\n== writing fixed-size binary records ==");
    if (!write_binary(records, count)) {
        return EXIT_FAILURE;
    }
    printf("  each record is %zu bytes, %zu records = %zu bytes\n",
           sizeof(Record), count, sizeof(Record) * count);

    puts("\n== seeking directly, without reading what precedes ==");
    Record one;
    for (size_t i = 0; i < count; i += 2) {
        if (read_record_at(i, &one)) {
            printf("  record %zu: ", i);
            print_record(&one);
        }
    }
    read_record_at(99, &one);

    puts("\n== buffering ==");
    printf("  this has no newline and may not appear yet...");
    fflush(stdout);
    puts(" [flushed]");
    fprintf(stderr, "  stderr is unbuffered, so this is never delayed\n");
    puts("  run './files > out.txt 2>&1' and compare the order");

    puts("\n== why while(!feof(f)) is wrong ==");
    FILE *f = fopen(TEXT_FILE, "r");
    if (f != NULL) {
        char line[LINE_LEN];
        int bad = 0;
        while (!feof(f)) {                 /* deliberately wrong */
            if (fgets(line, sizeof line, f) == NULL) break;
            bad++;
        }
        rewind(f);
        int good = 0;
        while (fgets(line, sizeof line, f) != NULL) {
            good++;
        }
        printf("  feof-driven loop needed a break to stay correct: %d\n", bad);
        printf("  return-value-driven loop: %d lines, no special case\n", good);
        fclose(f);
    }

    remove(TEXT_FILE);
    remove(BINARY_FILE);
    puts("\n  temporary files removed");
    return EXIT_SUCCESS;
}
