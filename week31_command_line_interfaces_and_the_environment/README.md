# Week 31 — Command-Line Interfaces and the Environment

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_31_Command_Line_Interfaces_and_Environment.html)** — read these first; the code here is the worked example from that page.

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
gcc -std=c17 -Wall -Wextra -g -fsanitize=address -o filter filter.c
```

```bash
./filter -h | head -3           # works: help is on stdout
./filter -x 2> errors.txt       # the error is captured, not the output
```
