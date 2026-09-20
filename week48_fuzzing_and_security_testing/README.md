# Week 48 — Fuzzing and Security Testing

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_48_Fuzzing_and_Security_Testing.html)** — read these first; the code here is the worked example from that page.

## Files

- `fuzz_parser.c`
- `parser.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
clang -g -O1 -fsanitize=fuzzer,address,undefined \
      -o fuzz_parser fuzz_parser.c parser.c
./fuzz_parser corpus/ -max_len=4096
```

```bash
mkdir corpus
printf 'timeout = 30\nname = server\n' > corpus/valid.conf
printf '# comment only\n'              > corpus/comment.conf
printf ''                              > corpus/empty.conf
```

```bash
./fuzz_parser crash-a1b2c3d4              # replays that one input
xxd crash-a1b2c3d4 | head
```

```bash
./fuzz_parser -minimize_crash=1 -runs=100000 crash-a1b2c3d4
```

```bash
cp crash-a1b2c3d4 tests/regressions/overflow-in-length-field
# the test suite replays every file in that directory
```

```bash
clang -g -O1 -fsanitize=fuzzer,address,undefined \
      -o fuzz_parser fuzz_parser.c parser.c

mkdir -p corpus
printf 'timeout = 30\nname = server\n' > corpus/valid
printf '# comment\n'                   > corpus/comment
printf 'values = 4\n'                  > corpus/values

./fuzz_parser corpus/ -max_len=4096 -print_final_stats=1
```
