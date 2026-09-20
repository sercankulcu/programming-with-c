#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- what this machine looks like ---------- */

static bool is_little_endian(void)
{
    uint32_t probe = 1;
    unsigned char first;
    memcpy(&first, &probe, 1);
    return first == 1;
}

/* ---------- explicit big-endian accessors ---------- */

static void store_be16(uint8_t *out, uint16_t v)
{
    out[0] = (uint8_t)(v >> 8);
    out[1] = (uint8_t)(v);
}
static void store_be32(uint8_t *out, uint32_t v)
{
    out[0] = (uint8_t)(v >> 24); out[1] = (uint8_t)(v >> 16);
    out[2] = (uint8_t)(v >>  8); out[3] = (uint8_t)(v);
}
static void store_be64(uint8_t *out, uint64_t v)
{
    for (int i = 0; i < 8; i++) {
        out[i] = (uint8_t)(v >> (56 - 8 * i));
    }
}
static uint16_t load_be16(const uint8_t *in)
{
    return (uint16_t)(((uint16_t)in[0] << 8) | in[1]);
}
static uint32_t load_be32(const uint8_t *in)
{
    return ((uint32_t)in[0] << 24) | ((uint32_t)in[1] << 16)
         | ((uint32_t)in[2] <<  8) |  (uint32_t)in[3];
}
static uint64_t load_be64(const uint8_t *in)
{
    uint64_t v = 0;
    for (int i = 0; i < 8; i++) v = (v << 8) | in[i];
    return v;
}

/* ---------- the in-memory record ---------- */

#define NAME_BYTES 32

typedef struct {
    uint32_t id;
    uint16_t score;
    uint64_t timestamp;
    char     name[NAME_BYTES];      /* UTF-8 bytes, NUL padded */
} Record;

/* ---------- the wire format: fixed offsets, fixed widths, big-endian ----------
 *   offset  size  field
 *        0     4  magic   "CREC"
 *        4     2  version
 *        6     4  id
 *       10     2  score
 *       12     8  timestamp
 *       20    32  name (UTF-8, NUL padded)
 *   total 52 bytes, identical on every machine
 */

#define WIRE_SIZE   52
#define WIRE_MAGIC  "CREC"
#define WIRE_VERSION 1

static void record_encode(const Record *r, uint8_t out[WIRE_SIZE])
{
    memcpy(out, WIRE_MAGIC, 4);
    store_be16(out + 4,  WIRE_VERSION);
    store_be32(out + 6,  r->id);
    store_be16(out + 10, r->score);
    store_be64(out + 12, r->timestamp);
    memset(out + 20, 0, NAME_BYTES);
    memcpy(out + 20, r->name, strnlen(r->name, NAME_BYTES));
}

static bool record_decode(const uint8_t in[WIRE_SIZE], Record *out)
{
    if (memcmp(in, WIRE_MAGIC, 4) != 0) {
        return false;                       /* not our format */
    }
    uint16_t version = load_be16(in + 4);
    if (version != WIRE_VERSION) {
        return false;                       /* version check, not a guess */
    }
    out->id        = load_be32(in + 6);
    out->score     = load_be16(in + 10);
    out->timestamp = load_be64(in + 12);
    memcpy(out->name, in + 20, NAME_BYTES);
    out->name[NAME_BYTES - 1] = '\0';       /* guarantee termination */
    return true;
}

/* ---------- UTF-8 ---------- */

static size_t utf8_length(const char *s)
{
    size_t chars = 0;
    for (; *s != '\0'; s++) {
        if (((unsigned char)*s & 0xC0) != 0x80) chars++;
    }
    return chars;
}

/* Truncate to at most max_bytes without splitting a character. */
static size_t utf8_safe_truncate(const char *s, size_t max_bytes)
{
    if (strlen(s) <= max_bytes) return strlen(s);
    size_t cut = max_bytes;
    while (cut > 0 && ((unsigned char)s[cut] & 0xC0) == 0x80) {
        cut--;                              /* back off to a lead byte */
    }
    return cut;
}

int main(void)
{
    puts("== this machine ==");
    printf("  CHAR_BIT assumed 8; sizeof: int %zu, long %zu, void * %zu\n",
           sizeof(int), sizeof(long), sizeof(void *));
    printf("  data model  : %s\n",
           sizeof(long) == 8 ? "LP64 (Linux/macOS)" :
           sizeof(void *) == 8 ? "LLP64 (Windows)" : "ILP32");
    printf("  byte order  : %s\n",
           is_little_endian() ? "little-endian" : "big-endian");
    printf("  alignment   : int %zu, double %zu, max %zu\n",
           _Alignof(int), _Alignof(double), _Alignof(max_align_t));
    printf("  plain char is %s\n", (char)-1 < 0 ? "signed" : "unsigned");

    puts("\n== why a raw struct is not a format ==");
    printf("  sizeof(Record) = %zu, but the fields total %zu\n",
           sizeof(Record), (size_t)(4 + 2 + 8 + NAME_BYTES));
    printf("  offsets: id %zu, score %zu, timestamp %zu, name %zu\n",
           offsetof(Record, id), offsetof(Record, score),
           offsetof(Record, timestamp), offsetof(Record, name));
    printf("  the wire format is a fixed %d bytes on every machine\n",
           WIRE_SIZE);

    puts("\n== encode and decode ==");
    Record original = {
        .id = 0x12345678u,
        .score = 950,
        .timestamp = UINT64_C(1735689600),
        .name = "Ada Lovelace"
    };

    uint8_t wire[WIRE_SIZE];
    record_encode(&original, wire);

    printf("  first 20 bytes: ");
    for (int i = 0; i < 20; i++) printf("%02X ", wire[i]);
    putchar('\n');
    puts("  note bytes 6-9 are 12 34 56 78 — most significant first,");
    puts("  regardless of how this machine stores integers internally");

    Record restored;
    if (record_decode(wire, &restored)) {
        printf("  id        : 0x%08" PRIX32 " %s\n", restored.id,
               restored.id == original.id ? "ok" : "MISMATCH");
        printf("  score     : %" PRIu16 " %s\n", restored.score,
               restored.score == original.score ? "ok" : "MISMATCH");
        printf("  timestamp : %" PRIu64 " %s\n", restored.timestamp,
               restored.timestamp == original.timestamp ? "ok" : "MISMATCH");
        printf("  name      : \"%s\"\n", restored.name);
    }

    puts("\n== rejecting foreign and future data ==");
    uint8_t alien[WIRE_SIZE] = { 'X','X','X','X' };
    printf("  wrong magic   : %s\n",
           record_decode(alien, &restored) ? "accepted (bad)" : "rejected");
    memcpy(alien, WIRE_MAGIC, 4);
    store_be16(alien + 4, 99);
    printf("  future version: %s\n",
           record_decode(alien, &restored) ? "accepted (bad)" : "rejected");

    puts("\n== unaligned access ==");
    uint8_t buffer[16] = { 0, 0x12, 0x34, 0x56, 0x78 };
    uint32_t value;
    memcpy(&value, buffer + 1, sizeof value);     /* the correct way */
    printf("  memcpy from an odd offset: 0x%08" PRIX32 "\n", value);
    puts("  *(uint32_t *)(buffer + 1) would be an alignment and");
    puts("  aliasing violation; it crashes on some architectures");

    puts("\n== UTF-8 ==");
    const char *turkish = "türkçe karakterler";
    printf("  \"%s\"\n", turkish);
    printf("  strlen      = %zu bytes\n", strlen(turkish));
    printf("  utf8_length = %zu characters\n", utf8_length(turkish));

    for (size_t limit = 1; limit <= 4; limit++) {
        size_t cut = utf8_safe_truncate(turkish, limit);
        printf("  truncate to %zu bytes -> %zu bytes: \"%.*s\"\n",
               limit, cut, (int)cut, turkish);
    }
    puts("  cutting at an arbitrary byte would split a character in half");

    return EXIT_SUCCESS;
}
