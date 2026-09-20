# Week 47 — Event-Driven and Scalable I/O

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_47_Event_Driven_and_Scalable_IO.html)** — read these first; the code here is the worked example from that page.

> **Platform:** POSIX only. This week's subject *is* the operating-system interface
> (`fork`, signals, sockets, `epoll`), which Windows does not provide in a form that would
> teach the same lesson. On Windows, run it under WSL — see week 01 of the notes.

## Files

- `evserver.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
gcc -std=c17 -D_GNU_SOURCE -Wall -Wextra -g -o evserver evserver.c
./evserver 8080 &
```
