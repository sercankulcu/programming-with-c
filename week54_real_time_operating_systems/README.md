# Week 54 — Real-Time Operating Systems

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_54_Real_Time_Operating_Systems.html)** — read these first; the code here is the worked example from that page.

> **Platform:** cross-compiled for ARM Cortex-M and run under QEMU. No hardware needed.
> Install `gcc-arm-none-eabi` and `qemu-system-arm`.

## Files

- `superloop.c` — the device without a scheduler; this is what `make` builds and `make run` starts
- `board.c`, `board.h` — the small board layer both versions share
- `rtos.c` — the FreeRTOS version from the notes (not built by default, see below)
- `superloop_sketch.c.txt` — the bare superloop from the notes, to read, not to compile

## Build and run

```bash
make
make run
```

## The FreeRTOS version

FreeRTOS is not bundled with this repository, so `rtos.c` needs one extra step.

```bash
git clone --depth 1 https://github.com/FreeRTOS/FreeRTOS-Kernel.git
make rtos.elf FREERTOS=$PWD/FreeRTOS-Kernel
```

A complete FreeRTOS build also needs a `FreeRTOSConfig.h`, the Cortex-M3 port
(`portable/GCC/ARM_CM3`) and a heap (`portable/MemMang/heap_4.c`) next to the kernel
sources (`tasks.c`, `queue.c`, `list.c`). The `rtos.elf` rule in the Makefile only
compiles `rtos.c` against the kernel headers; link the kernel, port and heap sources in
the same way to get a binary that runs under QEMU.
