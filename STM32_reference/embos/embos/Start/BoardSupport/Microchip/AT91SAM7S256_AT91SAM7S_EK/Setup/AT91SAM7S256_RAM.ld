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
File   : AT91SAM7S256_RAM.ld
Purpose: Link file for AT91SAM7S256 RAM
-------- END-OF-HEADER -----------------------------------------------
*/

ENTRY(__vector)

/*********************************************************************
*
*       Define stack sizes here
*/
FIQ_STACK_SIZE = 0x0;
IRQ_STACK_SIZE = 0x0100;
ABT_STACK_SIZE = 0x0;
UND_STACK_SIZE = 0x0;
SVC_STACK_SIZE = 0x0400;

MEMORY
{
  RAM (wx)  : ORIGIN = 0x00200000, LENGTH = 0x00010000
}

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
    *(.rodata);
    . = ALIGN(4);
    *(.rodata*);
    . = ALIGN(4);
    *(.glue_7t);
    . = ALIGN(4);
    *(.glue_7);
    . = ALIGN(4);
    etext = .;
  } > RAM

  . = ALIGN(4);
  _etext = . ;
  PROVIDE (etext = .);
   
  .data : AT (_etext)
  {
    PROVIDE (__data_start__ = .);
    _data = . ;
    *(.data)
    . = ALIGN(4);
    PROVIDE (__data_end__ = .);
  } > RAM

  . = ALIGN(4);
  _edata = . ;
  PROVIDE (edata = .);

  .bss :
  {
    PROVIDE (__bss_start__ = .);
    *(.bss)
    *(COMMON)
    . = ALIGN(4);
    PROVIDE (__bss_end__ = .);
    
    . = ALIGN(256);
    
    PROVIDE (__stack_start__ = .);
    
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

    PROVIDE (__stack_svc_start__ = .);
    . += SVC_STACK_SIZE;
    . = ALIGN(8);
    PROVIDE (__stack_svc_end__ = .);
    PROVIDE (__stack_end__ = .);
    PROVIDE (__heap_start__ = .);   
  } > RAM
}

