# Procedural Programming with C — worked examples

Every compilable program from the 56-week course
**[Procedural Programming with C](https://sercankulcu.github.io/teaching/programming-with-c/)**,
one directory per week, each building on its own.

The lecture notes are the course; this repository is the code from them. Each
directory's `README.md` links back to the week it came from.

```bash
git clone https://github.com/sercankulcu/programming-with-c
cd programming-with-c
make week01        # build one week
make portable      # build every week that is plain C
make clean
```

## Layout

```
week01_computers_programs_and_the_c_toolchain/
├── README.md     link to the notes, what the files are, experiments to run
├── Makefile
└── main.c
```

## Platforms

Most of this is plain standard C and builds anywhere. Where it is not, the
reason is that the *subject of that week is the operating system*, and the
Windows equivalent would teach a different lesson rather than the same one in
another dialect.

| Group | Weeks | Linux | macOS | Windows |
|---|---|:-:|:-:|:-:|
| Portable C | 1–28, 30, 32–38, 42–43, 45, 56 | ✅ | ✅ | ✅ native (MinGW/MSVC) |
| POSIX | 31, 39, 40, 41, 47, 48 | ✅ | ✅ | WSL |
| Cross-compiled | 49–55 | ✅ QEMU | ✅ QEMU | WSL |
| Multi-language | 46 | ✅ | ✅ | WSL |

**On Windows**, install WSL once and you have everything:

```bash
wsl --install
```

This is what week 01 of the notes recommends, and it is the environment the rest
of the course assumes. For the portable weeks you can also stay native with
MinGW-w64, MSYS2 or MSVC — CI builds them that way on every push, precisely to
keep the portability claims in week 37 honest.

## Toolchain

```bash
# Debian / Ubuntu / WSL
sudo apt install build-essential gdb valgrind clang
sudo apt install gcc-arm-none-eabi qemu-system-arm   # weeks 49-55

# macOS
xcode-select --install
brew install qemu arm-none-eabi-gcc
```

## Two weeks have no program

Week 29 is about Make, CMake and libraries, and week 44 is about reviewing
somebody else's change. Their directories hold the build files and the
before/after diff instead of a program to run.

## Licence

MIT for the code. The lecture notes on the site are a separate work.
