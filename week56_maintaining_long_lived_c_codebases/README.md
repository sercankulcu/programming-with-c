# Week 56 — Maintaining Long-Lived C Codebases

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_56_Maintaining_Long_Lived_Codebases.html)** — read these first; the code here is the worked example from that page.

## Files

- `characterize.c`
- `legacy.c`
- `legacy.h`
- `parser.c`
- `test_legacy.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
gcc -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes ..
```

```bash
gcc -O2 -S -w legacy.c -o legacy.s
gcc -O2 -S -w parser.c -o parser.s
diff legacy.s parser.s        # identical means the behavior cannot have changed
```

```bash
gcc -std=c17 -O2 -fsanitize=undefined ..      # run the suite under UBSan
clang -std=c17 -O2 ..                          # a second opinion
gcc -fanalyzer ..                              # week 35's static analysis
```

```bash
gcc -w -o characterize characterize.c legacy.c && ./characterize > pinned.h
```

```bash
gcc -std=c17 -Wall -Wextra -Wstrict-prototypes -Wold-style-definition \
    -g -fsanitize=address,undefined -c parser.c -o parser.o
./test_legacy
```

```bash
git commit -m "Modernize parse_record without changing behavior"
git commit -m "Reject malformed values instead of returning 0

parse_record() returned 0 for 'x = abc', indistinguishable from
'x = 0'. Callers cannot detect the difference. Now returns false.

Updates the two pinned tests that encoded the old behavior, and the
three call sites that relied on it. Closes #412."
```
