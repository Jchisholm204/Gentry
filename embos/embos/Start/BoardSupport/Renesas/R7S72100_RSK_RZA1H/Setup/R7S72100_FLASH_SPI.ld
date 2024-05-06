/*********************************************************************
*
*       Define stack sizes here
*/
FIQ_STACK_SIZE = 0x0;
IRQ_STACK_SIZE = 0x0100;
ABT_STACK_SIZE = 0x0100;
UND_STACK_SIZE = 0x0;
SVC_STACK_SIZE = 0x0800;

_Min_Heap_Size = 0x1000;

ENTRY (__vector)

MEMORY
{
  /* Executable and data to be placed in memory. */
  FLASH  (rx)  : ORIGIN = 0x18000000, LENGTH = 0x4000000  /* 64MB SPI flash */
  RAM    (rwx) : ORIGIN = 0x60020000, LENGTH = 0x9E0000   /* Leave out the first 128kB of the 10MB SRAM as it is used as
                                                            On-Chip Data Retention RAM */
}

SECTIONS{
    .vectors :
    {
        *(.vectors)
    } >FLASH

    .text :
    {
        *(.text)
    } >FLASH
    .init :
    {
        *(.init)
    } >FLASH
    .fini :
    {
        *(.fini)
    } >FLASH
    .got :
    {
        *(.got)
        *(.got.plt)
    } >FLASH
    .rodata :
    {
        *(.rodata)
        *(.rodata.*)
        _erodata = .;
    } >FLASH
    .eh_frame_hdr :
    {
        *(.eh_frame_hdr)
    } >FLASH
    .eh_frame :
    {
        *(.eh_frame)
    } >FLASH
    .jcr :
    {
        *(.jcr)
    } >FLASH
    .tors :
    {
        *(.ctors)
        _mdata = .;
    } >FLASH

    .ARM.extab   : {
      *(.ARM.extab* .gnu.linkonce.armextab.*)
  } >FLASH

  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  . = ALIGN(8);
  .data : ALIGN(8) {
    /* Global symbol at start of data */
    __data_start__ = .;
    __data_load__ = LOADADDR(.data);
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    /* Global symbol at end of data */
    __data_end__ = .;
  } >RAM AT>FLASH

/************************************************************************************
 *
 *  "no init" section holds variables that do not need to be initialized
 *  or have to be initialized by __low_level_init() and therefore must not be
 *  overwritten by segment initialization in crt0
 *  Default location: Goes into RAM, location does not matter
 *
 ************************************************************************************/

  .NoInit (NOLOAD) :
  {
    . = ALIGN(8);
    *(.no_init)
  } >RAM

  .bss :
  {
    _bss = .;
    PROVIDE(__bss_start__ = .);
    *(.bss)
    *(.bss.**)
    *(COMMON)
    . = ALIGN(4);
    PROVIDE(__bss_end__ = .);
    _ebss = .;
    _end = .;
  } >RAM

  .heap (NOLOAD) :
  {
    . = ALIGN(8);
    end = .;
    __heap_start__ = .;
    . = . + _Min_Heap_Size;
    __heap_end__ = .;
    . = ALIGN(8);
  } >RAM

  .stack (NOLOAD) :
  {
    . = ALIGN(8);
    PROVIDE (__stack_fiq_start__ = .);
    . += FIQ_STACK_SIZE;
    . = ALIGN(8);
    PROVIDE (__stack_fiq_end__ = .);

    PROVIDE (__stack_irq_start__ = .);
    . += IRQ_STACK_SIZE;
    . = ALIGN(8);
    PROVIDE (__stack_irq_end__ = .);

    PROVIDE (__stack_abt_start__ = .);
    . += ABT_STACK_SIZE;
    . = ALIGN(8);
    PROVIDE (__stack_abt_end__ = .);

    PROVIDE (__stack_und_start__ = .);
    . += UND_STACK_SIZE;
    . = ALIGN(8);
    PROVIDE (__stack_und_end__ = .);

    __stack_start__ = .;
    PROVIDE (__stack_svc_start__ = .);
    . += SVC_STACK_SIZE;
    . = ALIGN(8);
    PROVIDE (__stack_svc_end__ = .);
    __stack_end__ = .;
    _stack = .;
  } >RAM
}
