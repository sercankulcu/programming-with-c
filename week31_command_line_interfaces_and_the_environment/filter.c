#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>

#define VERSION "1.0"
#define EXIT_USAGE 2

static const char *program_name = "filter";

typedef struct {
    bool        verbose;
    long        maximum;
    const char *output_path;
    const char *prefix;
} Config;

static void usage(FILE *to)
{
    fprintf(to,
        "usage: %s [options] [file...]\n"
        "\n"
        "  -v            verbose diagnostics on stderr\n"
        "  -n LIMIT      stop after LIMIT lines (default 0 = unlimited)\n"
        "  -o FILE       write to FILE instead of stdout\n"
        "  -p PREFIX     prefix every output line\n"
        "  -h            show this help and exit\n"
        "  -V            show the version and exit\n"
        "\n"
        "  With no file operands, or with '-', reads standard input.\n"
        "  Use '--' to end options before a filename beginning with '-'.\n"
        "\n"
        "Environment:\n"
        "  FILTER_PREFIX   default for -p\n"
        "  FILTER_MAX      default for -n\n"
        "\n"
        "Exit status: 0 success, 1 runtime error, %d usage error.\n",
        program_name, EXIT_USAGE);
}

static bool parse_long(const char *text, long *out)
{
    if (text == NULL || *text == '\0') return false;
    errno = 0;
    char *end;
    long value = strtol(text, &end, 10);
    if (end == text || *end != '\0' || errno == ERANGE) return false;
    *out = value;
    return true;
}

/* Layer 1: built-in defaults. */
static void config_defaults(Config *c)
{
    c->verbose     = false;
    c->maximum     = 0;
    c->output_path = NULL;
    c->prefix      = "";
}

/* Layer 3: the environment, overriding defaults. */
static bool config_from_env(Config *c)
{
    const char *prefix = getenv("FILTER_PREFIX");
    if (prefix != NULL) {
        c->prefix = prefix;
    }

    const char *max_text = getenv("FILTER_MAX");
    if (max_text != NULL) {
        long value;
        if (!parse_long(max_text, &value) || value < 0) {
            fprintf(stderr, "%s: FILTER_MAX is not a valid count: \"%s\"\n",
                    program_name, max_text);
            return false;          /* environment input is validated too */
        }
        c->maximum = value;
    }
    return true;
}

/* A registered cleanup handler. */
static char temp_marker[64];
static void on_exit_cleanup(void)
{
    if (temp_marker[0] != '\0') {
        remove(temp_marker);
    }
}

static bool process_stream(FILE *in, FILE *out, const Config *c,
                           const char *label, long *emitted)
{
    char line[512];
    long count = 0;

    while (fgets(line, sizeof line, in) != NULL) {
        if (c->maximum > 0 && *emitted >= c->maximum) {
            break;
        }
        if (strchr(line, '\n') == NULL && !feof(in)) {
            fprintf(stderr, "%s: %s: line too long, truncating\n",
                    program_name, label);
            int ch;
            while ((ch = fgetc(in)) != '\n' && ch != EOF) { }
        }
        line[strcspn(line, "\n")] = '\0';

        if (fprintf(out, "%s%s\n", c->prefix, line) < 0) {
            perror("write");
            return false;
        }
        count++;
        (*emitted)++;
    }

    if (ferror(in)) {
        fprintf(stderr, "%s: %s: read error\n", program_name, label);
        return false;
    }
    if (c->verbose) {
        fprintf(stderr, "%s: %s: %ld lines\n", program_name, label, count);
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (argc > 0 && argv[0] != NULL) {
        program_name = argv[0];
    }
    atexit(on_exit_cleanup);

    Config cfg;
    config_defaults(&cfg);                       /* layer 1 */
    if (!config_from_env(&cfg)) {                /* layer 3 */
        return EXIT_USAGE;
    }

    /* Layer 4: command-line options, highest priority. */
    opterr = 0;                                  /* our messages, not getopt's */
    int opt;
    while ((opt = getopt(argc, argv, ":vn:o:p:hV")) != -1) {
        switch (opt) {
        case 'v':
            cfg.verbose = true;
            break;
        case 'n':
            if (!parse_long(optarg, &cfg.maximum) || cfg.maximum < 0) {
                fprintf(stderr, "%s: -n needs a non-negative number, got \"%s\"\n",
                        program_name, optarg);
                return EXIT_USAGE;
            }
            break;
        case 'o':
            cfg.output_path = optarg;
            break;
        case 'p':
            cfg.prefix = optarg;
            break;
        case 'h':
            usage(stdout);                       /* help goes to stdout */
            return EXIT_SUCCESS;
        case 'V':
            printf("%s %s\n", program_name, VERSION);
            return EXIT_SUCCESS;
        case ':':
            fprintf(stderr, "%s: option -%c requires an argument\n",
                    program_name, optopt);
            usage(stderr);                       /* errors go to stderr */
            return EXIT_USAGE;
        case '?':
        default:
            fprintf(stderr, "%s: unknown option -%c\n", program_name, optopt);
            usage(stderr);
            return EXIT_USAGE;
        }
    }

    if (cfg.verbose) {
        fprintf(stderr, "%s: config: verbose=%d max=%ld out=%s prefix=\"%s\"\n",
                program_name, cfg.verbose, cfg.maximum,
                cfg.output_path ? cfg.output_path : "<stdout>", cfg.prefix);
    }

    FILE *out = stdout;
    if (cfg.output_path != NULL) {
        out = fopen(cfg.output_path, "w");
        if (out == NULL) {
            fprintf(stderr, "%s: %s: %s\n",
                    program_name, cfg.output_path, strerror(errno));
            return EXIT_FAILURE;
        }
    }

    int status = EXIT_SUCCESS;
    long emitted = 0;

    if (optind == argc) {
        /* No operands: read standard input, as a filter should. */
        if (!process_stream(stdin, out, &cfg, "-", &emitted)) {
            status = EXIT_FAILURE;
        }
    } else {
        for (int i = optind; i < argc; i++) {
            if (strcmp(argv[i], "-") == 0) {
                if (!process_stream(stdin, out, &cfg, "-", &emitted)) {
                    status = EXIT_FAILURE;
                }
                continue;
            }
            FILE *in = fopen(argv[i], "r");
            if (in == NULL) {
                fprintf(stderr, "%s: %s: %s\n",
                        program_name, argv[i], strerror(errno));
                status = EXIT_FAILURE;
                continue;                        /* keep going, like grep */
            }
            if (!process_stream(in, out, &cfg, argv[i], &emitted)) {
                status = EXIT_FAILURE;
            }
            fclose(in);
        }
    }

    if (out != stdout && fclose(out) != 0) {
        perror("closing output");
        status = EXIT_FAILURE;
    }
    return status;
}
