/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
*                           www.segger.com                           *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------
File: STM32L073VZ_Flash.ld

*/

ENTRY(Reset_Handler)

__heap_size__  = 0x400;
__stack_size__ = 0x400;

__dataload_start__ = LOADADDR(.data);
__vectors_start__  = ADDR(.vectors);

MEMORY
{
  FLASH (xr ) : ORIGIN = 0x00000000, LENGTH = 64K
  RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 16K
}

SECTIONS
{
  . = ALIGN(4);
  .vectors :
  {
    KEEP(*(.vectors))
  } >FLASH

  . = ALIGN(4);
  .text :
  {
    *(.text)
    *(.text*)
    KEEP(*(.init))
    KEEP(*(.fini))
    . = ALIGN(4);
    _etext = .;
  } >FLASH

  . = ALIGN(4);
  .rodata :
  {
    *(.rodata)
    *(.rodata*)
  } >FLASH

  . = ALIGN(4);
  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } >FLASH

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  .preinit_array :
  {
    PROVIDE_HIDDEN(__preinit_array_start = .);
    KEEP(*(.preinit_array*))
    PROVIDE_HIDDEN(__preinit_array_end = .);
  } >FLASH

  .init_array :
  {
    PROVIDE_HIDDEN(__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array*))
    PROVIDE_HIDDEN(__init_array_end = .);
  } >FLASH

  .fini_array :
  {
    PROVIDE_HIDDEN(__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array*))
    PROVIDE_HIDDEN(__fini_array_end = .);
  } >FLASH

  . = ALIGN(4);
  .data :
  {
    __data_start__ = .;
    *(.data)
    *(.data.*)
    .          = ALIGN(4);
    __data_end__   = .;
  } >RAM AT> FLASH

  .bss ALIGN(4) :
  {
    __bss_start__ = .;
    *(.bss)
    *(.bss.*)
    *(COMMON)
    .             = ALIGN(4);
    __bss_end__   = .;
  } >RAM

  . = ALIGN(4);
  .heap :
  {
    __heap_start__ = .;
    .              = . + __heap_size__;
    __heap_end__   = .;
  } >RAM

  . = ALIGN(4);
  .stack :
  {
    __stack_start__ = .;
    .               = . + __stack_size__;
    __stack_end__   = .;
  } >RAM

  /DISCARD/ :
  {
    libc.a   (*)
    libm.a   (*)
    libgcc.a (*)
  }

  .ARM.attributes 0 :
  {
    *(.ARM.attributes)
  }
}


