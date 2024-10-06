/*********************************************************************
*               SEGGER MICROCONTROLLER GmbH & Co KG                  *
*       Solutions for real time microcontroller applications         *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2013  SEGGER Microcontroller GmbH & Co KG         *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system for microcontrollers      *
*                                                                    *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product or a real-time            *
*       operating system for in-house use.                           *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       OS version: Internal                                         *
*                                                                    *
*       Current version number will be inserted here                 *
*       when shipment is built.                                      *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File   : AT91SAM7S256_FLASH.ld
Purpose: Link file for AT91SAM7S256 FLASH
-------- END-OF-HEADER -----------------------------------------------
*/

/*********************************************************************
*
*       Memory layout
*/

MEMORY
{
  RAM   (wx)  : ORIGIN = 0x00200000, LENGTH = 0x00010000
  FLASH (rx)  : ORIGIN = 0x00100000, LENGTH = 0x00040000
}

/*********************************************************************
*
*       Define stack and heap sizes here
*/

FIQ_STACK_SIZE = 0x0;
IRQ_STACK_SIZE = 0x0100;
ABT_STACK_SIZE = 0x0;
UND_STACK_SIZE = 0x0;
SVC_STACK_SIZE = 0x0400;
_Min_Heap_Size = 0x1000;   /* Amount of heap for the application */

/*********************************************************************
*
*       Code and data sections
*/

ENTRY(__vector)

SECTIONS
{
  .text :
  {
    *(.vectors);
    . = ALIGN(4);
    *(.init);
    . = ALIGN(4);
    *(.text);
    . = ALIGN(4);
    *(.text*);
    . = ALIGN(4);
    *(.rodata);
    . = ALIGN(4);
    *(.rodata*);
    . = ALIGN(4);
    *(.glue_7t);
    . = ALIGN(4);
    *(.glue_7);
    . = ALIGN(4);
    KEEP (*(.init))
    KEEP (*(.fini))
  } > FLASH

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } >FLASH

  __exidx_start = .;
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } >FLASH
  __exidx_end = .;

  .ARM.attributes :
  {
    *(.ARM.attributes)
  } >FLASH

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(.fini_array*))
    KEEP (*(SORT(.fini_array.*)))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > FLASH

  __etext = .;

/*********************************************************************
*
*       Data section
*       Goes into RAM, loads in FLASH
*       __data_start__, __data_load__ and __data_end__ are needed by
*       the startup file
*/

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
  } >RAM AT>FLASH

/*********************************************************************
*
*       "no init" section holds variables that do not need to be initialized
*       or have to be initialized by __low_level_init() and therefore 
*       must not be overwritten by segment initialization in crt0
*       Default location: Goes into RAM, location does not matter
*/

  . = ALIGN(8);
  .NoInit (NOLOAD) : ALIGN(8)
  {
    *(.no_init)
  } >RAM

/*********************************************************************
*
*       bss section (Uninitialized data)
*       Goes into RAM
*/
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

/*********************************************************************
*
*       user_heap_stack section
*       Check that there is enough RAM left for stack and heap
*/

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

/*********************************************************************
*
*       Remove information from the standard libraries
*/
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }
}

/****** End Of File *************************************************/
