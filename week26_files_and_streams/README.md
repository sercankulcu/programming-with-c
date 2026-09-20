# Week 26 — Files and Streams

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_26_Files_and_Streams.html)** — read these first; the code here is the worked example from that page.

## Files

- `files.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
gcc -std=c17 -Wall -Wextra -g -fsanitize=address -o files files.c
./files
```

```bash
./files            # stdout to a terminal: line buffered
./files > out.txt 2>&1
cat out.txt        # different interleaving of stdout and stderr
```
