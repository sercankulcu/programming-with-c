# Week 39 — Processes, Signals, and Low-Level I/O

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_39_Processes_Signals_and_Low_Level_IO.html)** — read these first; the code here is the worked example from that page.

> **Platform:** POSIX only. This week's subject *is* the operating-system interface
> (`fork`, signals, sockets, `epoll`), which Windows does not provide in a form that would
> teach the same lesson. On Windows, run it under WSL — see week 01 of the notes.

## Files

- `minishell.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
gcc -std=c17 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -g -o minishell minishell.c
./minishell
```
