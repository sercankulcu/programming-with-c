#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netdb.h>

#define MAX_EVENTS   64
#define BUF_SIZE   4096
#define IDLE_LIMIT   30      /* seconds */

typedef struct {
    int     fd;
    char    in[BUF_SIZE];
    size_t  in_used;
    char    out[BUF_SIZE];
    size_t  out_used;
    size_t  out_sent;
    long    lines;
    time_t  last_active;
    bool    closing;
} Connection;

static Connection *conns[65536];
static int  epfd     = -1;
static int  wake[2]  = { -1, -1 };
static long live     = 0;
static long peak     = 0;
static long total    = 0;

/* ---------- signal handling via the self-pipe trick ---------- */

static void on_signal(int sig)
{
    (void)sig;
    char b = 1;
    ssize_t r = write(wake[1], &b, 1);    /* the only safe call here */
    (void)r;
}

static bool set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) return false;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK) == 0;
}

/* ---------- connection lifecycle ---------- */

static void conn_close(Connection *c)
{
    epoll_ctl(epfd, EPOLL_CTL_DEL, c->fd, NULL);
    close(c->fd);
    conns[c->fd] = NULL;
    free(c);
    live--;
}

/* Ask for writability only while there is something to write. */
static void conn_update_interest(Connection *c)
{
    struct epoll_event ev = { .data.fd = c->fd, .events = EPOLLIN };
    if (c->out_used > c->out_sent) {
        ev.events |= EPOLLOUT;
    }
    epoll_ctl(epfd, EPOLL_CTL_MOD, c->fd, &ev);
}

static void conn_queue(Connection *c, const char *data, size_t n)
{
    if (c->out_used + n > BUF_SIZE) {      /* client not draining */
        c->closing = true;
        return;
    }
    memcpy(c->out + c->out_used, data, n);
    c->out_used += n;
}

/* Framing, exactly as in week 40: a stream has no message boundaries. */
static void conn_process_input(Connection *c)
{
    char *start = c->in;
    char *nl;
    while ((nl = memchr(start, '\n', (size_t)(c->in + c->in_used - start)))) {
        size_t len = (size_t)(nl - start);
        c->lines++;

        if (len == 4 && memcmp(start, "quit", 4) == 0) {
            conn_queue(c, "bye\n", 4);
            c->closing = true;
            break;
        }

        char reply[BUF_SIZE];
        int n = snprintf(reply, sizeof reply, "%ld: %.*s\n",
                         c->lines, (int)len, start);
        if (n > 0) conn_queue(c, reply, (size_t)n);
        start = nl + 1;
    }

    size_t leftover = (size_t)(c->in + c->in_used - start);
    memmove(c->in, start, leftover);
    c->in_used = leftover;

    if (c->in_used == BUF_SIZE) {          /* a line longer than the buffer */
        conn_queue(c, "line too long\n", 14);
        c->closing = true;
    }
}

static void conn_readable(Connection *c)
{
    for (;;) {
        if (c->in_used == BUF_SIZE) break;

        ssize_t n = recv(c->fd, c->in + c->in_used,
                         BUF_SIZE - c->in_used, 0);
        if (n > 0) {
            c->in_used += (size_t)n;
            c->last_active = time(NULL);
            conn_process_input(c);
            if (c->closing) break;
            continue;                      /* drain: there may be more */
        }
        if (n == 0) {                      /* orderly close */
            c->closing = true;
            c->out_used = c->out_sent;     /* nothing left to deliver */
            break;
        }
        if (errno == EAGAIN || errno == EWOULDBLOCK) break;   /* normal */
        if (errno == EINTR) continue;
        c->closing = true;                 /* a real error */
        break;
    }
}

static void conn_writable(Connection *c)
{
    while (c->out_sent < c->out_used) {
        ssize_t n = send(c->fd, c->out + c->out_sent,
                         c->out_used - c->out_sent, MSG_NOSIGNAL);
        if (n > 0) {
            c->out_sent += (size_t)n;
            c->last_active = time(NULL);
            continue;
        }
        if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) return;
        if (n < 0 && errno == EINTR) continue;
        c->closing = true;
        return;
    }
    c->out_used = c->out_sent = 0;         /* buffer drained */
}

static void accept_new(int listener)
{
    for (;;) {                             /* accept until EAGAIN */
        int fd = accept(listener, NULL, NULL);
        if (fd < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) return;
            if (errno == EINTR) continue;
            perror("accept");
            return;
        }
        if (!set_nonblocking(fd)) { close(fd); continue; }

        Connection *c = calloc(1, sizeof *c);
        if (c == NULL) { close(fd); continue; }
        c->fd = fd;
        c->last_active = time(NULL);
        conns[fd] = c;

        struct epoll_event ev = { .events = EPOLLIN, .data.fd = fd };
        if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) < 0) {
            close(fd); free(c); conns[fd] = NULL; continue;
        }

        conn_queue(c, "ready\n", 6);
        conn_update_interest(c);

        live++; total++;
        if (live > peak) peak = live;
    }
}

static void sweep_idle(void)
{
    time_t now = time(NULL);
    for (int fd = 0; fd < 65536; fd++) {
        Connection *c = conns[fd];
        if (c != NULL && now - c->last_active > IDLE_LIMIT) {
            conn_close(c);
        }
    }
}

int main(int argc, char *argv[])
{
    const char *port = (argc > 1) ? argv[1] : "8080";

    if (pipe(wake) < 0) { perror("pipe"); return EXIT_FAILURE; }
    set_nonblocking(wake[0]);
    set_nonblocking(wake[1]);

    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = on_signal;
    sa.sa_flags   = SA_RESTART;
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    signal(SIGPIPE, SIG_IGN);

    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    struct addrinfo *list;
    int rc = getaddrinfo(NULL, port, &hints, &list);
    if (rc != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return EXIT_FAILURE;
    }

    int listener = socket(list->ai_family, list->ai_socktype,
                          list->ai_protocol);
    int yes = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
    if (bind(listener, list->ai_addr, list->ai_addrlen) < 0) {
        perror("bind"); return EXIT_FAILURE;
    }
    freeaddrinfo(list);

    set_nonblocking(listener);
    listen(listener, 512);

    epfd = epoll_create1(0);
    struct epoll_event ev = { .events = EPOLLIN, .data.fd = listener };
    epoll_ctl(epfd, EPOLL_CTL_ADD, listener, &ev);
    ev.data.fd = wake[0];
    epoll_ctl(epfd, EPOLL_CTL_ADD, wake[0], &ev);

    printf("event loop on port %s, one thread, Ctrl-C to stop\n", port);

    struct epoll_event events[MAX_EVENTS];
    bool running = true;
    time_t last_sweep = time(NULL);

    while (running) {
        int n = epoll_wait(epfd, events, MAX_EVENTS, 1000);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < n; i++) {
            int fd = events[i].data.fd;

            if (fd == listener) {
                accept_new(listener);
                continue;
            }
            if (fd == wake[0]) {           /* a signal arrived */
                char drain[64];
                while (read(wake[0], drain, sizeof drain) > 0) { }
                running = false;
                continue;
            }

            Connection *c = conns[fd];
            if (c == NULL) continue;

            if (events[i].events & (EPOLLHUP | EPOLLERR)) {
                conn_close(c);
                continue;
            }
            if (events[i].events & EPOLLIN)  conn_readable(c);
            if (events[i].events & EPOLLOUT) conn_writable(c);

            if (c->closing && c->out_sent >= c->out_used) {
                conn_close(c);              /* flush before closing */
            } else {
                conn_update_interest(c);
            }
        }

        if (time(NULL) - last_sweep >= 5) {
            sweep_idle();
            last_sweep = time(NULL);
        }
    }

    for (int fd = 0; fd < 65536; fd++) {
        if (conns[fd] != NULL) conn_close(conns[fd]);
    }
    close(listener);
    close(epfd);
    close(wake[0]);
    close(wake[1]);

    printf("\nserved %ld connections, peak concurrent %ld\n", total, peak);
    return EXIT_SUCCESS;
}
