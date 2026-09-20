/* firmware.ld — week 52 covers this properly */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

_stack_top = ORIGIN(RAM) + LENGTH(RAM);     /* stack grows down from the top */

SECTIONS
{
    .text : {
        KEEP(*(.vectors))       /* the vector table must be first */
        *(.text*)
        *(.rodata*)             /* constants stay in flash */
    } > FLASH

    _data_load = LOADADDR(.data);

    .data : {
        _data_start = .;
        *(.data*)
        _data_end = .;
    } > RAM AT> FLASH           /* lives in RAM, stored in FLASH */

    .bss : {
        _bss_start = .;
        *(.bss*)
        *(COMMON)
        _bss_end = .;
    } > RAM
}
