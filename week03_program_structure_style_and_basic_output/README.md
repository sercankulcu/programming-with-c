# Week 03 — Program Structure, Style, and Basic Output

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_03_Program_Structure_and_Style.html)** — read these first; the code here is the worked example from that page.

## Files

- `structure.c` — the annotated skeleton
- `hello.c` — the same program with nothing added
- `ugly.c` — the version this week asks you to read
- `clean.c` — the same program, made readable

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
./hello
echo $?
```

```bash
./myprogram && echo "worked"
./myprogram || echo "failed"
```

```bash
clang-format -style=llvm -i hello.c
```

```bash
gcc -std=c17 -Wall -Wextra -g -o ugly ugly.c
gcc -std=c17 -Wall -Wextra -g -o clean clean.c
./ugly > a.txt
./clean | grep -o '[0-9]*$' > b.txt
diff a.txt b.txt && echo "same numbers"
```

```bash
mkdir c-course && cd c-course
git init
git config user.name  "Your Name"
git config user.email "you@example.com"
```

```bash
git status                       # what changed
git add week03/clean.c           # stage this file
git commit -m "Week 3: readable rewrite of the grid example"
git log --oneline                # what has happened so far
```
