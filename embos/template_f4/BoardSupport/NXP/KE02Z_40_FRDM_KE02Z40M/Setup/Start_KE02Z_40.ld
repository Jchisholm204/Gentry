/*
 * GENERATED FILE - DO NOT EDIT
 * (c) Code Red Technologies Ltd, 2008-2013
 * (c) NXP Semiconductors 2013-2018
 * Generated linker script file for MKE02Z64xxx4
 * Created from linkscript.ldt by FMCreateLinkLibraries
 * Using Freemarker v2.3.23
 * MCUXpresso IDE v10.2.0 [Build 759] [2018-05-15]  on Aug 20, 2018 9:47:21 AM
 */

GROUP (
  "libgcc.a"
  "libc_nano.a"
  "libm.a"
)

MEMORY
{
  /* Define each memory region */
  PROGRAM_FLASH (rx)  : ORIGIN = 0x0,        LENGTH = 0x10000 /* 64K bytes (alias Flash) */
  SRAM          (rwx) : ORIGIN = 0x1ffffc00, LENGTH = 0x1000  /*  4K bytes (alias RAM)   */
}

  /* Define a symbol for the top of each memory region */
  __base_PROGRAM_FLASH = 0x0;                  /* PROGRAM_FLASH */
  __base_Flash         = 0x0;                  /* Flash */
  __top_PROGRAM_FLASH  = 0x0 + 0x10000;        /* 64K bytes */
  __top_Flash          = 0x0 + 0x10000;        /* 64K bytes */
  __base_SRAM          = 0x1ffffc00;           /* SRAM */
  __base_RAM           = 0x1ffffc00;           /* RAM */
  __top_SRAM           = 0x1ffffc00 + 0x1000;  /* 4K bytes */
  __top_RAM            = 0x1ffffc00 + 0x1000;  /* 4K bytes */

  /* Define heap and stack size */
  _HeapSize  = 0x400;
  _StackSize = 0x400;

ENTRY(ResetISR)

SECTIONS
{
    /* MAIN TEXT SECTION */
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        _vectors = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        /* Global Section Table */
        . = ALIGN(4) ;
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        __bss_section_table_end = .;
        __section_table_end = . ;
        /* End of Global Section Table */


        /* Kinetis Flash Configuration data */
        . = 0x400;
        PROVIDE(__FLASH_CONFIG_START__ = .) ;
        KEEP(*(.FlashConfig))
        PROVIDE(__FLASH_CONFIG_END__ = .) ;
        ASSERT(!(__FLASH_CONFIG_START__ == __FLASH_CONFIG_END__), "Linker Flash Config Support Enabled, but no .FlashConfig section provided within application");
        /* End of Kinetis Flash Configuration data */
        *(.after_vectors*)

    } > PROGRAM_FLASH

    .text : ALIGN(4)
    {
       *(.text*)
       *(.rodata .rodata.* .constdata .constdata.*)
       . = ALIGN(4);
    } > PROGRAM_FLASH
    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this.
     */
    .ARM.extab : ALIGN(4)
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > PROGRAM_FLASH

    __exidx_start = .;

    .ARM.exidx : ALIGN(4)
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > PROGRAM_FLASH
    __exidx_end = .;

    _etext = .;

    /* MAIN DATA SECTION */
    .uninit_RESERVED : ALIGN(4)
    {
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > SRAM

    /* Main DATA section (SRAM) */
    .data : ALIGN(4)
    {
       FILL(0xff)
       _data = . ;
       *(vtable)
       *(.ramfunc*)
       *(.data*)
       . = ALIGN(4) ;
       _edata = . ;
    } > SRAM AT>PROGRAM_FLASH

    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(end = .);
    } > SRAM

    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        *(.noinit*)
         . = ALIGN(4) ;
        _end_noinit = .;
    } > SRAM

    /* Reserve and place Heap within memory map */
    .heap :  ALIGN(4)
    {
        _pvHeapStart = .;
        __heap_start__ = _pvHeapStart;
        . += _HeapSize;
        . = ALIGN(4);
        _pvHeapLimit = .;
        __heap_end__ = _pvHeapLimit;
    } > SRAM

     /* Reserve space in memory for Stack */
    .heap2stackfill  :
    {
        . += _StackSize;
    } > SRAM
    /* Locate actual Stack in memory map */
    .stack ORIGIN(SRAM) + LENGTH(SRAM) - _StackSize - 0:  ALIGN(4)
    {
        _vStackBase = .;
        __stack_start__ = _vStackBase;
        . = ALIGN(4);
        _vStackTop = . + _StackSize;
        __stack_end__ = _vStackTop;
    } > SRAM

    /* Provide basic symbols giving location and size of main text
     * block, including initial values of RW data sections. Note that
     * these will need extending to give a complete picture with
     * complex images (e.g multiple Flash banks).
     */
    _image_start = LOADADDR(.text);
    _image_end = LOADADDR(.data) + SIZEOF(.data);
    _image_size = _image_end - _image_start;
}