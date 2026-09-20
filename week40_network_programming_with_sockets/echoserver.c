/* echoserver.c — a TCP server, one forked child per connection */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>

#define BACKLOG      16
#define MAX_LINE   4096

static volatile sig_atomic_t stop = 0;

static void on_signal(int sig) { (void)sig; stop = 1; }

/* Reap children so they do not become zombies — week 39. */
static void on_child(int sig)
{
    (void)sig;
    int saved = errno;                      /* waitpid may change errno */
    while (waitpid(-1, NULL, WNOHANG) > 0) { }
    errno = saved;
}

static ssize_t send_all(int fd, const void *data, size_t n)
{
    const char *p = data;
    size_t sent = 0;
    while (sent < n) {
        ssize_t s = send(fd, p + sent, n - sent, MSG_NOSIGNAL);
        if (s < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        sent += (size_t)s;
    }
    return (ssize_t)sent;
}

/* Newline framing: accumulate until a '\n' appears. A stream has no
   message boundaries, so this loop is what creates them. */
static void serve_connection(int fd)
{
    char    buffer[MAX_LINE];
    size_t  used = 0;
    long    lines = 0;

    struct timeval tv = { .tv_sec = 30, .tv_usec = 0 };
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);

    const char *greeting = "ready\n";
    if (send_all(fd, greeting, strlen(greeting)) < 0) return;

    for (;;) {
        ssize_t r = recv(fd, buffer + used, sizeof buffer - used, 0);

        if (r == 0) {
            break;                          /* orderly close: not an error */
        }
        if (r < 0) {
            if (errno == EINTR) continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                send_all(fd, "timeout\n", 8);
            }
            break;
        }
        used += (size_t)r;

        /* One recv may contain zero, one, or several complete lines. */
        char *start = buffer;
        char *nl;
        while ((nl = memchr(start, '\n', (size_t)(buffer + used - start)))) {
            size_t len = (size_t)(nl - start);
            lines++;

            if (len == 4 && memcmp(start, "quit", 4) == 0) {
                send_all(fd, "bye\n", 4);
                return;
            }

            char out[MAX_LINE + 32];
            int n = snprintf(out, sizeof out, "%ld: %.*s\n",
                             lines, (int)len, start);
            if (n > 0 && send_all(fd, out, (size_t)n) < 0) return;

            start = nl + 1;
        }

        /* Move the incomplete remainder to the front. */
        size_t leftover = (size_t)(buffer + used - start);
        memmove(buffer, start, leftover);
        used = leftover;

        if (used == sizeof buffer) {        /* a line longer than the buffer */
            send_all(fd, "line too long\n", 14);
            return;
        }
    }
}

int main(int argc, char *argv[])
{
    const char *port = (argc > 1) ? argv[1] : "8080";

    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = on_signal;
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    memset(&sa, 0, sizeof sa);
    sa.sa_handler = on_child;
    sa.sa_flags   = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);

    signal(SIGPIPE, SIG_IGN);               /* a closed peer must not kill us */

    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;         /* for bind */

    struct addrinfo *list;
    int rc = getaddrinfo(NULL, port, &hints, &list);
    if (rc != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return EXIT_FAILURE;
    }

    int listener = -1;
    for (struct addrinfo *ai = list; ai != NULL; ai = ai->ai_next) {
        listener = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (listener < 0) continue;

        int yes = 1;
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

        if (bind(listener, ai->ai_addr, ai->ai_addrlen) == 0) break;
        close(listener);
        listener = -1;
    }
    freeaddrinfo(list);

    if (listener < 0) {
        perror("bind");
        return EXIT_FAILURE;
    }
    if (listen(listener, BACKLOG) < 0) {
        perror("listen");
        return EXIT_FAILURE;
    }

    printf("listening on port %s; Ctrl-C to stop\n", port);

    while (!stop) {
        int conn = accept(listener, NULL, NULL);
        if (conn < 0) {
            if (errno == EINTR) continue;   /* a signal, not a failure */
            perror("accept");
            break;
        }

        pid_t pid = fork();
        if (pid == 0) {
            close(listener);                /* the child does not need it */
            serve_connection(conn);
            close(conn);
            _exit(0);                       /* _exit in a child */
        }
        close(conn);                        /* the parent does not need it */
        if (pid < 0) perror("fork");
    }

    close(listener);
    puts("\nstopped");
    return EXIT_SUCCESS;
}
