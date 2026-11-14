/* test_protocol.c — builds and runs on the host */
#include "protocol.h"
#include "fake_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int checks = 0, failures = 0;

#define CHECK(cond) do {                                          \
    checks++;                                                     \
    if (!(cond)) { failures++;                                    \
        printf("  FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); }\
    else printf("  ok   %s\n", #cond);                            \
} while (0)

static void test_single_line(void)
{
    puts("\n-- one line --");
    Fake f; Hal hal; Protocol p;
    fake_init(&f, "hello\n", &hal);
    protocol_init(&p, &hal);

    CHECK(protocol_poll(&p) == PROTO_OK);
    CHECK(strcmp(f.output, "1:hello\n") == 0);
    CHECK(p.lines_handled == 1);
}

static void test_split_across_polls(void)
{
    puts("\n-- a line arriving in pieces --");
    Fake f; Hal hal; Protocol p;
    fake_init(&f, "hel", &hal);
    protocol_init(&p, &hal);

    protocol_poll(&p);
    CHECK(f.output_len == 0);            /* nothing yet: no newline */

    f.input = "lo\n"; f.input_pos = 0;   /* the rest arrives */
    protocol_poll(&p);
    CHECK(strcmp(f.output, "1:hello\n") == 0);
}

static void test_overflow(void)
{
    puts("\n-- a line longer than the buffer --");
    char big[PROTO_MAX_LINE + 20];
    memset(big, 'x', sizeof big - 2);
    big[sizeof big - 2] = '\n';
    big[sizeof big - 1] = '\0';

    Fake f; Hal hal; Protocol p;
    fake_init(&f, big, &hal);
    protocol_init(&p, &hal);

    CHECK(protocol_poll(&p) == PROTO_OVERFLOW);
    CHECK(strstr(f.output, "overflow") != NULL);
    CHECK(p.used == 0);                  /* reset, not left half-full */
}

static void test_timeout(void)
{
    puts("\n-- a partial line that goes quiet --");
    Fake f; Hal hal; Protocol p;
    fake_init(&f, "partial", &hal);
    protocol_init(&p, &hal);

    f.now = 100;
    CHECK(protocol_poll(&p) == PROTO_OK);    /* not yet */

    f.now = 100 + 2000;                      /* time advances instantly */
    CHECK(protocol_poll(&p) == PROTO_TIMEOUT);
    CHECK(strstr(f.output, "timeout") != NULL);
}

static void test_several_lines_one_poll(void)
{
    puts("\n-- three lines in one burst --");
    Fake f; Hal hal; Protocol p;
    fake_init(&f, "a\nb\nc\n", &hal);
    protocol_init(&p, &hal);

    protocol_poll(&p);
    CHECK(strcmp(f.output, "1:a\n2:b\n3:c\n") == 0);
    CHECK(p.lines_handled == 3);
}

int main(void)
{
    puts("host tests for the protocol layer");
    test_single_line();
    test_split_across_polls();
    test_overflow();
    test_timeout();
    test_several_lines_one_poll();

    printf("\n%d checks, %d failed\n", checks, failures);
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
