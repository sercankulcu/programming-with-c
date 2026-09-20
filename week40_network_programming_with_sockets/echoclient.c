/* echoclient.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

static ssize_t send_all(int fd, const void *data, size_t n)
{
    const char *p = data;
    size_t sent = 0;
    while (sent < n) {
        ssize_t s = send(fd, p + sent, n - sent, MSG_NOSIGNAL);
        if (s < 0) { if (errno == EINTR) continue; return -1; }
        sent += (size_t)s;
    }
    return (ssize_t)sent;
}

int main(int argc, char *argv[])
{
    const char *host = (argc > 1) ? argv[1] : "localhost";
    const char *port = (argc > 2) ? argv[2] : "8080";

    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *list;
    int rc = getaddrinfo(host, port, &hints, &list);
    if (rc != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return EXIT_FAILURE;
    }

    int fd = -1;
    for (struct addrinfo *ai = list; ai != NULL; ai = ai->ai_next) {
        fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (fd < 0) continue;
        if (connect(fd, ai->ai_addr, ai->ai_addrlen) == 0) break;
        close(fd);
        fd = -1;
    }
    freeaddrinfo(list);

    if (fd < 0) { perror("connect"); return EXIT_FAILURE; }

    struct timeval tv = { .tv_sec = 10, .tv_usec = 0 };
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);

    char line[1024];
    while (fgets(line, sizeof line, stdin) != NULL) {
        if (send_all(fd, line, strlen(line)) < 0) { perror("send"); break; }

        char reply[2048];
        ssize_t r = recv(fd, reply, sizeof reply - 1, 0);
        if (r < 0)  { perror("recv"); break; }
        if (r == 0) { puts("server closed the connection"); break; }
        reply[r] = '\0';
        fputs(reply, stdout);
    }

    close(fd);
    return EXIT_SUCCESS;
}
