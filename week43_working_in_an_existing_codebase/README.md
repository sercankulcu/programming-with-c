# Week 43 — Working in an Existing Codebase

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_43_Working_in_an_Existing_Codebase.html)** — read these first; the code here is the worked example from that page.

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
cloc .                          # size, languages, where the weight is
ls -R src include | head -50
git log --oneline -20           # what is being worked on now
git shortlog -sn | head         # who to ask
wc -l src/*.c | sort -n | tail  # the biggest files are usually the core
```

```bash
git bisect start
git bisect bad                  # the current commit is broken
git bisect good v2.1            # this one worked
# git checks out a commit in the middle
make && ./run_the_test
git bisect good                 # or: git bisect bad
# repeat until git names the commit
git bisect reset                # return to where you started
```

```bash
git bisect start HEAD v2.1
git bisect run ./check.sh       # exit 0 = good, non-zero = bad
```

```bash
#!/bin/sh
# check.sh — exit 0 if the build is good
make clean && make >/dev/null 2>&1 || exit 125   # 125 = skip, untestable
./prog test_input | grep -q "expected output"
```

```bash
mkdir -p bisect-demo && cd bisect-demo && git init -q
```

```bash
./check.sh; echo "check says $?"      # non-zero under a sanitizer build

git bisect start HEAD HEAD~7
git bisect run ./check.sh
```
