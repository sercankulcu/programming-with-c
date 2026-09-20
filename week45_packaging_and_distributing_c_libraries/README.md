# Week 45 — Packaging and Distributing C Libraries

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_45_Packaging_and_Distributing_Libraries.html)** — read these first; the code here is the worked example from that page.

## Files

- `intarray.h`
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
gcc -shared -Wl,-soname,libintarray.so.2 \
    -o libintarray.so.2.3.0 intarray.o

ln -sf libintarray.so.2.3.0 libintarray.so.2      # runtime link
ln -sf libintarray.so.2     libintarray.so        # build-time link
```

```bash
gcc -fvisibility=hidden -fPIC -c intarray.c       # hide by default
```

```bash
gcc -shared -Wl,--version-script=intarray.map -o libintarray.so.2.3.0 ..
```

```bash
nm -D --defined-only libintarray.so.2.3.0 | grep ' T '
```

```bash
gcc app.c $(pkg-config --cflags --libs intarray) -o app
pkg-config --modversion intarray
pkg-config --atleast-version=2.1 intarray && echo "new enough"
```

```bash
make
make check
```
