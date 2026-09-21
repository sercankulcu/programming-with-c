# Procedural Programming with C — worked examples

[![build](https://github.com/sercankulcu/programming-with-c/actions/workflows/ci.yml/badge.svg)](https://github.com/sercankulcu/programming-with-c/actions/workflows/ci.yml)

Every compilable program from the 56-week course
**[Procedural Programming with C](https://sercankulcu.github.io/teaching/procedural-programming-with-c)**,
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
| Portable C | 1–28, 30, 32–38, 42–43, 45, 55–56 | ✅ | ✅ | ✅ native (MinGW/MSVC) |
| POSIX | 31, 39–41 | ✅ | ✅ | WSL |
| Linux-only | 47 (`epoll`), 48 (libFuzzer) | ✅ | — | WSL |
| Cross-compiled | 49–54 | ✅ QEMU | ✅ QEMU | WSL |
| Multi-language | 46 | ✅ | ✅ | WSL |

Weeks 47 and 48 are the two that will not build anywhere else: `epoll` is a
Linux interface with no macOS equivalent that teaches the same thing, and the
libFuzzer that ships with Apple's clang cannot link a fuzz target. Their
makefiles say so and stop rather than failing obscurely.

Most weeks compile as `-std=c17`. The ones using POSIX interfaces use
`-std=gnu17` instead: strict ISO mode hides `getopt`, `sigaction`,
`getaddrinfo` and `clock_gettime` behind `__STRICT_ANSI__`, and the
`_POSIX_C_SOURCE` macro that opens them on glibc *restricts* them on macOS.

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

## Two weeks have no program yet

Week 29 is about Make, CMake and building libraries, and week 44 is about
reviewing somebody else's change. Neither has a program to run, and their
directories are currently empty apart from a README pointing at the notes —
the makefiles and the worked review from those weeks have not been written up
here yet.

## Licence

MIT for the code. The lecture notes on the site are a separate work.
