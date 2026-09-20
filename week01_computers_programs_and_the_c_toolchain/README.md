# Week 01 — Computers, Programs, and the C Toolchain

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_01_Computers_and_Toolchain.html)** — read these first; the code here is the worked example from that page.

## Files

- `hello.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
sudo apt update
sudo apt install build-essential gdb
```

```bash
xcode-select --install
```

```bash
wsl --install
```

```bash
gcc --version
```

```bash
gcc -Wall -Wextra -g -o hello hello.c
./hello
```
