# Week 41 — Concurrency and the C Memory Model

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_41_Concurrency_and_Memory_Model.html)** — read these first; the code here is the worked example from that page.

> **Platform:** POSIX only. This week's subject *is* the operating-system interface
> (`fork`, signals, sockets, `epoll`), which Windows does not provide in a form that would
> teach the same lesson. On Windows, run it under WSL — see week 01 of the notes.

## Files

- `main.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
gcc -std=c17 -Wall -Wextra -g -pthread -o concurrent concurrent.c
./concurrent
```

```bash
gcc -std=c17 -Wall -Wextra -g -pthread -fsanitize=thread -o concurrent_tsan concurrent.c
./concurrent_tsan
```

```bash
gdb -p $(pgrep concurrent)
(gdb) thread apply all bt
```
