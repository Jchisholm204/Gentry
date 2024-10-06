/************************************************************************************
 *  File:     Flash.ld
 *  Purpose:  Linker script to configure memory regions for ARM7/ARM7TDMI/ARM7TDMI-S
 *            and place sections and symbol values.
 *  Date:     05 July 2013
 *  Info:     Reset_Handler must be defined in code as entry point of reset handler.
 *
 *            By default the script is configured for the first flash and RAM bank
 *            of the device.
 *            If additional banks of the device shall be used,
 *            they can be configured manually.
 *
 *            Following ymbols are defined in the script
 *            and can be used without definition in code:
 *            __exidx_start
 *            __exidx_end
 *            __etext
 *            __data_start__
 *            __preinit_array_start
 *            __preinit_array_end
 *            __init_array_start
 *            __init_array_end
 *            __fini_array_start
 *            __fini_array_end
 *            __data_end__
 *            __bss_start__
 *            __bss_end__
 *            __stack_end__
 ************************************************************************************/

/************************************************************************************
 *
 *  Memory regions and required amount of heap and stack
 ************************************************************************************/
MEMORY
{
  RAM (xrw)       : ORIGIN = 0x00000000, LENGTH = 0x00040000
}


FIQ_STACK_SIZE = 0x0;
IRQ_STACK_SIZE = 0x0100;
ABT_STACK_SIZE = 0x0;
UND_STACK_SIZE = 0x0;
SVC_STACK_SIZE = 0x400;
_Min_Heap_Size = 0x1000;   /* Amount of heap for the application */

/************************************************************************************
 *
 *  Code and data sections
 ************************************************************************************/

ENTRY(__vector)

SECTIONS
{
/************************************************************************************
 *
 *  vectors and text section (code)
 *  Starts with startup code and vectors
 *  Goes into RAM
 ************************************************************************************/

  . = ALIGN(4);
  .vectors : ALIGN(4)
  {
    KEEP(*(.vectors)) /* Startup code */
    . = ALIGN(4);
  } >RAM

  . = ALIGN(4);
  .text : ALIGN(4)
  {
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */

    KEEP (*(.init))
    KEEP (*(.fini))
  } >RAM

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } >RAM

  __exidx_start = .;
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } >RAM
  __exidx_end = .;

  .ARM.attributes :
  {
    *(.ARM.attributes)
  } >RAM

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >RAM
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >RAM
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(.fini_array*))
    KEEP (*(SORT(.fini_array.*)))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > RAM

  __etext = .;

/************************************************************************************
 *
 *  data section
 *  Goes into RAM, loads in RAM
 ************************************************************************************/

  . = ALIGN(8);
  .data : ALIGN(8)
  {
    /* Global symbol at start of data */
    __data_start__ = .;
    __data_load__ = LOADADDR(.data);
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    /* Global symbol at end of data */
    __data_end__ = .;
  } >RAM

/************************************************************************************
 *
 *  "no init" section holds variables that do not need to be initialized
 *  or have to be initialized by __low_level_init() and therefore must not be
 *  overwritten by segment initialization in crt0
 *  Default location: Goes into RAM, location does not matter
 ************************************************************************************/

  . = ALIGN(8);
  .NoInit (NOLOAD) : ALIGN(8)
  {
    *(.no_init)
  } >RAM

/************************************************************************************
 *
 *  bss section (Uninitialized data)
 *  Goes into RAM
 ************************************************************************************/

  . = ALIGN(8);
  .bss : ALIGN(8)
  {
    /* Global symbol at start of bss */
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    /* Global symbol at end of bss */
    _ebss = .;
    __bss_end__ = _ebss;
  } >RAM

/************************************************************************************
 *
 *  user_heap_stack section
 *  Check that there is enough RAM left for stack and heap
 ************************************************************************************/

  . = ALIGN(8);
  ._user_heap_stack (NOLOAD) : ALIGN(8)
  {
    __heap_start__ = .;
    . = . + _Min_Heap_Size;
    __heap_end__ = .;
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
  } >RAM

/************************************************************************************
 *
 *  Remove information from the standard libraries
 ************************************************************************************/
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }
}
