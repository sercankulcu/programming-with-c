# Week 37 — Portability and the Machine Model

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_37_Portability_and_Machine_Model.html)** — read these first; the code here is the worked example from that page.

## Files

- `portable.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
gcc -std=c17 -Wall -Wextra -g -fsanitize=address,undefined -o portable portable.c
./portable
```

```bash
./portable | grep -A1 'first 20 bytes'
xxd -l 20 <<< ""            # for comparison with a raw struct dump
```

```bash
qemu-s390x -L /usr/s390x-linux-gnu ./portable_s390x   # big-endian target
```
