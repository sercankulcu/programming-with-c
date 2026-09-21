#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>

/* Set by the handler, read by the loop. Both qualifiers are required. */
static volatile sig_atomic_t interrupted = 0;

static void on_sigint(int sig)
{
    (void)sig;
    interrupted = 1;
    /* Nothing else. write() would be safe; printf() would not. */
}

/* read and write may transfer less than requested; loop. */
static ssize_t write_all(int fd, const void *data, size_t n)
{
    const char *p = data;
    size_t written = 0;
    while (written < n) {
        ssize_t w = write(fd, p + written, n - written);
        if (w < 0) {
            if (errno == EINTR) continue;      /* retry, not an error */
            return -1;
        }
        written += (size_t)w;
    }
    return (ssize_t)written;
}

static void describe_status(const char *what, int status)
{
    if (WIFEXITED(status)) {
        printf("  %s exited with %d\n", what, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("  %s killed by signal %d\n", what, WTERMSIG(status));
    } else {
        printf("  %s ended in an unexpected way\n", what);
    }
}

/* ---------- 1. fork and exec one command ---------- */

static int run_command(char *const argv[])
{
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }
    if (pid == 0) {                      /* child */
        execvp(argv[0], argv);
        perror(argv[0]);                 /* only on failure */
        _exit(127);                      /* _exit, not exit */
    }

    int status;
    while (waitpid(pid, &status, 0) < 0) {
        if (errno != EINTR) { perror("waitpid"); return -1; }
    }
    return status;
}

/* ---------- 2. redirect output to a file ---------- */

static int run_redirected(char *const argv[], const char *path)
{
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return -1; }

    if (pid == 0) {
        int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) { perror(path); _exit(126); }
        if (dup2(fd, STDOUT_FILENO) < 0) { perror("dup2"); _exit(126); }
        close(fd);                        /* the copy in fd 1 remains */
        execvp(argv[0], argv);
        perror(argv[0]);
        _exit(127);
    }

    int status;
    waitpid(pid, &status, 0);
    return status;
}

/* ---------- 3. connect two commands with a pipe ---------- */

static int run_pipeline(char *const left[], char *const right[])
{
    int fds[2];
    if (pipe(fds) < 0) { perror("pipe"); return -1; }

    pid_t first = fork();
    if (first < 0) { perror("fork"); return -1; }
    if (first == 0) {
        close(fds[0]);                    /* not reading */
        dup2(fds[1], STDOUT_FILENO);
        close(fds[1]);
        execvp(left[0], left);
        perror(left[0]);
        _exit(127);
    }

    pid_t second = fork();
    if (second < 0) { perror("fork"); return -1; }
    if (second == 0) {
        close(fds[1]);                    /* not writing */
        dup2(fds[0], STDIN_FILENO);
        close(fds[0]);
        execvp(right[0], right);
        perror(right[0]);
        _exit(127);
    }

    /* The parent must close BOTH ends, or the reader never sees EOF. */
    close(fds[0]);
    close(fds[1]);

    int s1, s2;
    waitpid(first,  &s1, 0);
    waitpid(second, &s2, 0);
    describe_status("left", s1);
    describe_status("right", s2);
    return s2;
}

int main(void)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = on_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa, NULL) < 0) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    printf("parent pid %ld\n\n", (long)getpid());

    puts("== 1. fork: two processes, one program ==");
    pid_t pid = fork();
    if (pid == 0) {
        printf("  child : pid %ld, fork returned 0, parent is %ld\n",
               (long)getpid(), (long)getppid());
        _exit(3);
    }
    int status;
    waitpid(pid, &status, 0);
    printf("  parent: fork returned %ld\n", (long)pid);
    describe_status("child", status);

    puts("\n== 2. memory is copied, descriptors are shared ==");
    int shared = 100;
    pid = fork();
    if (pid == 0) {
        shared = 999;
        printf("  child  sees shared = %d\n", shared);
        _exit(0);
    }
    waitpid(pid, NULL, 0);
    printf("  parent sees shared = %d  (the child's change did not reach us)\n",
           shared);

    puts("\n== 3. exec replaces the program ==");
    char *echo_args[] = { "echo", "  hello from execvp", NULL };
    describe_status("echo", run_command(echo_args));

    puts("\n== 4. a command that does not exist ==");
    char *missing[] = { "definitely_not_a_command", NULL };
    describe_status("missing", run_command(missing));
    puts("  127 is the conventional code for 'command not found'");

    puts("\n== 5. redirection with dup2 ==");
    char *date_args[] = { "date", "+%Y-%m-%d", NULL };
    run_redirected(date_args, "shell_out.txt");
    FILE *f = fopen("shell_out.txt", "r");
    if (f != NULL) {
        char line[128];
        if (fgets(line, sizeof line, f)) printf("  file contains: %s", line);
        fclose(f);
    }
    remove("shell_out.txt");

    puts("\n== 6. a pipeline: seq 1 20 | grep 1 ==");
    char *left[]  = { "seq", "1", "20", NULL };
    char *right[] = { "grep", "1", NULL };
    run_pipeline(left, right);

    puts("\n== 7. low-level write, looped ==");
    const char *msg = "  written with write(2), not printf\n";
    write_all(STDOUT_FILENO, msg, strlen(msg));

    puts("\n== 8. signals ==");
    puts("  press Ctrl-C within three seconds to interrupt the loop");
    for (int i = 0; i < 30 && !interrupted; i++) {
        struct timespec ts = { .tv_sec = 0, .tv_nsec = 100000000L };
        nanosleep(&ts, NULL);
    }
    if (interrupted) {
        puts("  the handler set a flag; this message is printed out here,");
        puts("  where printf is safe — inside a handler it would not be");
    } else {
        puts("  no interrupt arrived");
    }

    puts("\n== 9. zombies ==");
    pid = fork();
    if (pid == 0) _exit(0);
    printf("  child %ld has exited but is not yet reaped: it is a zombie\n",
           (long)pid);
    puts("  run 'ps -el | grep defunct' in another terminal now");
    sleep(1);
    waitpid(pid, NULL, 0);
    puts("  reaped; the process table entry is gone");

    return EXIT_SUCCESS;
}
