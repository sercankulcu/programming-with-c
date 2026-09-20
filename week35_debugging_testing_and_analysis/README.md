# Week 35 — Debugging, Testing, and Analysis

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_35_Debugging_and_Testing.html)** — read these first; the code here is the worked example from that page.

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
gcc -std=c17 -Wall -Wextra -g -O0 prog.c -o prog     # -g, and -O0 for sanity
gdb ./prog
```

```bash
gcc -fanalyzer -Wall -Wextra -c prog.c      # GCC 10+, built in
clang --analyze prog.c
cppcheck --enable=all --std=c17 prog.c
scan-build make                              # clang, whole project
```

```bash
gcc --coverage -O0 -o tests tests.c lib.c
./tests
gcov lib.c
# or, readable:
lcov --capture --directory . --output-file cov.info
genhtml cov.info --output-directory report
```

```bash
gcc -std=c17 -Wall -Wextra -g -O1 -fsanitize=address,undefined -o tests tests.c
./tests ; echo "exit status $?"
```

```bash
gcc -std=c17 --coverage -O0 -o tests_cov tests.c
./tests_cov
gcov tests.c | head -5
less tests.c.gcov      # lines marked ##### were never executed
```

```bash
./tests                      # FAIL: m->count was 3, expected 2
gdb ./tests
(gdb) break map_remove
(gdb) run
(gdb) finish
(gdb) print m->count
```
