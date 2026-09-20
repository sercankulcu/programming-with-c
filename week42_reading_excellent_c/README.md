# Week 42 — Reading Excellent C

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_42_Reading_Excellent_C.html)** — read these first; the code here is the worked example from that page.

## Files

- `bench.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
git clone --depth 1 git://git.musl-libc.org/musl
cd musl
wc -l src/string/strlen.c
cat src/string/strlen.c
```

```bash
gcc -O2 -o bench bench.c && ./bench
```

```bash
git clone --depth 1 https://github.com/redis/redis
sed -n '1,120p' redis/src/sds.h        # a length-prefixed string type
sed -n '1,80p'  redis/src/dict.h       # an incrementally rehashing hash table
```

```bash
grep -rn "goto cleanup\|goto fail\|goto err" src/ | head
grep -rn "_create\|_free\|_destroy\|_new\|_release" src/*.h | head
grep -rn "zmalloc\|sqlite3_malloc\|xmalloc" src/ | head -5
ls include/ 2>/dev/null || grep -l "^[a-z].*(" src/*.h | head
```
