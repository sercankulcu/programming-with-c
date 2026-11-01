#!/bin/sh
# Run every embedded week under QEMU and check what it prints on the UART.
#
# ci/build.sh EMBEDDED proves the images link, but a wrong vector table or
# linker script links just as well. Here each image runs for a few seconds
# on the lm3s6965evb machine and passes only if a line its week promises
# appears on the serial port. None of the programs exit - they all end in a
# loop - so `timeout` stopping QEMU is the normal end of a run.
#
# Every week in the EMBEDDED group must have a line in the table below, so a
# new embedded week cannot slip through unrun. rtos.elf needs
# FREERTOS=/path/to/FreeRTOS-Kernel in the environment; week 54's Makefile
# picks it up from there.
set -u
summary="${GITHUB_STEP_SUMMARY:-/dev/stdout}"
label="${RUNNER_OS:-local}/${CC:-cc}"
secs="${QEMU_SECONDS:-8}"
tmp=$(mktemp -d)
failed=""

# week, image, what to type on the serial port (- for nothing),
# and the text that must appear (the rest of the line). Week 51's line is
# printed only after the UART interrupt has delivered the 'q'.
table() {
    cat <<'EOF'
week49_embedded_c_and_cross_compilation          firmware.elf  -   tick 2
week50_memory_mapped_i_o_and_hardware_registers  firmware.elf  aq  bye
week51_interrupts_and_real_time_behavior         firmware.elf  axq the loop kept running
week52_linker_scripts_and_firmware_memory_layout firmware.elf  -   .bss zeroed
week53_constrained_resource_programming          firmware.elf  -   constant timing
week54_real_time_operating_systems               superloop.elf -   WORST sensor gap
week54_real_time_operating_systems               rtos.elf      -   free heap
EOF
}

printf '\n## EMBEDDED under QEMU — %s\n\n| Week | Image | Result |\n|---|---|---|\n' "$label" >> "$summary"

for d in $(make -s print-EMBEDDED); do
    if ! table | grep -q "^$d "; then
        failed="$failed $d"
        printf '| `%s` | - | **no expected output in ci/qemu.sh** |\n' "$d" >> "$summary"
    fi
done

table > "$tmp/table"
while read -r d elf input want; do
    log="$tmp/$d-$elf.log"
    [ "$input" = - ] && input=""

    if ! make -C "$d" "$elf" < /dev/null > "$log" 2>&1; then
        failed="$failed $d/$elf"
        printf '| `%s` | %s | **BUILD FAILED** |\n' "$d" "$elf" >> "$summary"
        cat "$log"
        continue
    fi

    # The input goes in one character a second once QEMU is up, so week 51
    # sees its first byte while polling and the rest with interrupts on.
    # stdin then stays open until timeout ends the run.
    { sleep 1
      printf '%s\n' "$input" | fold -w1 | while IFS= read -r c; do
          printf '%s' "$c"; sleep 1
      done
      sleep "$secs"; } \
        | timeout "$secs" qemu-system-arm -M lm3s6965evb -nographic \
              -monitor none -serial stdio -kernel "$d/$elf" > "$log" 2>&1

    printf '\n==> %s/%s (looking for "%s")\n' "$d" "$elf" "$want"
    cat "$log"
    if grep -qF -- "$want" "$log"; then
        printf '| `%s` | %s | ok |\n' "$d" "$elf" >> "$summary"
    else
        failed="$failed $d/$elf"
        printf '| `%s` | %s | **FAILED**: no "%s" |\n' "$d" "$elf" "$want" >> "$summary"
    fi
done < "$tmp/table"

[ -z "$failed" ] && { printf '\nAll embedded images ran on %s.\n' "$label"; exit 0; }

printf '::error title=EMBEDDED under QEMU failed on %s::%s\n' "$label" "$failed"
printf '\nFailed:%s\n' "$failed"
exit 1
