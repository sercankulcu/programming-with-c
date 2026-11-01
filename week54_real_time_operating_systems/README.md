# Week 54 — Real-Time Operating Systems

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_54_Real_Time_Operating_Systems.html)** — read these first; the code here is the worked example from that page.

> **Platform:** cross-compiled for ARM Cortex-M and run under QEMU. No hardware needed.
> Install `gcc-arm-none-eabi` and `qemu-system-arm`.

## Files

- `superloop.c` — the device without a scheduler; this is what `make` builds and `make run` starts
- `tick.c` — the superloop's 1 ms SysTick handler (linked into `superloop.elf` only)
- `board.c`, `board.h` — the small board layer both versions share
- `startup.c`, `firmware.ld` — vector table, reset handler and memory layout, carried forward from weeks 49–53; the vector table now has the SVCall, PendSV and SysTick entries a scheduler needs
- `rtos.c` — the FreeRTOS version from the notes (not built by default, see below)
- `FreeRTOSConfig.h` — the kernel configuration for `rtos.c` on QEMU's lm3s6965evb
- `rtos_hooks.c` — what `configASSERT`, stack-overflow checking and a failed allocation report on the UART
- `superloop_sketch.c.txt` — the bare superloop from the notes, to read, not to compile

## Build and run

```bash
make
make run
```

## The FreeRTOS version

FreeRTOS is not bundled with this repository, so `rtos.c` needs one extra step.

```bash
git clone --depth 1 --branch V11.3.3 https://github.com/FreeRTOS/FreeRTOS-Kernel.git
make rtos.elf FREERTOS=$PWD/FreeRTOS-Kernel
make run-rtos FREERTOS=$PWD/FreeRTOS-Kernel
```

`make rtos.elf` links `rtos.c` with the kernel (`tasks.c`, `queue.c`, `list.c`,
`timers.c`), the Cortex-M3 port (`portable/GCC/ARM_CM3/port.c`), the `heap_4.c`
allocator, this week's `startup.c` and `firmware.ld`, and the `FreeRTOSConfig.h`
in this directory. `FreeRTOSConfig.h` renames the port's SVCall, PendSV and SysTick
handlers to the names in `startup.c`'s vector table, so the kernel owns SysTick in
`rtos.elf` while `tick.c` owns it in `superloop.elf`. The tag above is the one CI
builds and runs; other V11 releases should work as well.
