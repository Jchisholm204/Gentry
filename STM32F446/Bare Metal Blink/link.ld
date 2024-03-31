ENTRY(_reset);
MEMORY {
    /* f446 memory mapping (table 3*/
    flash(rx) : ORIGIN = 0x08000000, LENGTH = 512K
    sram(rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

_estack     = ORIGIN(sram) + LENGTH(sram); /* stack points to end of sram*/

SECTIONS {
    .vectors : { KEEP(*(.vectors)) } > flash
    .text    : { *(.text*) }         > flash
    .rodata  : { *(.rodata*) }       > flash

    .data : {
        _sdata = .; /*data section start */
        *(.first_data)
        *(.data SORT(.data*))
        _edata = .; /*data section end*/
    } > sram AT > flash

    _sidata = LOADADDR(.data);

    .bss : {
        _sbss = .;              /* .bss section start */
        *(.bss SORT(.bss.*) COMMON)
        _ebss = .;              /* .bss section end */
    } > sram
}