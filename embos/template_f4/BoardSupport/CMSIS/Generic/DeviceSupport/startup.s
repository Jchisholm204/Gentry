/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2023 SEGGER Microcontroller GmbH                  *
*                                                                    *
*       Internet: segger.com  Support: support_embos@segger.com      *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system                           *
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
*       OS version: V5.18.2.0                                        *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : startup.s
Purpose : Generic startup code and vector table for Cortex M3 and GCC
--------  END-OF-HEADER  ---------------------------------------------
*/

    .syntax unified
    .cpu cortex-m3
    .fpu softvfp
    .thumb

.global g_pfnVectors
.global Default_Handler
.global __Vectors

/* Linker script definitions */
/* start address for the initialization values of the .data section */
.word   _sidata
/* start address for the .data section */
.word   _sdata
/* end address for the .data section */
.word   _edata
/* start address for the .bss section */
.word   _sbss
/* end address for the .bss section */
.word   _ebss

/**
**===========================================================================
**  Program - Reset_Handler
**  Abstract: This code gets called after a reset event.
**    1. Call system initialzation routine
**    2. Copy .data section from ROM to RAM
**    3. Clear .bss section (Zero init)
**    4. Run static constructors
**    5. Enter main
**    6. Loop forever if returning from main
**===========================================================================
*/
    .section    .text.Reset_Handler
    .weak   Reset_Handler
    .type   Reset_Handler, %function
Reset_Handler:
    ldr   r0, =__stack_end__
    mov   sp, r0          /* set stack pointer */

    /* 1. Call system initialzation routine */
    bl SystemInit


    /* 2. copy .data section (Copy from ROM to RAM) */
    movs r1, #0
    b LoopCopyDataInit

CopyDataInit:
    ldr r3, =_sidata
    ldr r3, [r3, r1]
    str r3, [r0, r1]
    adds r1, r1, #4

LoopCopyDataInit:
    ldr r0, =_sdata
    ldr r3, =_edata
    adds r2, r0, r1
    cmp r2, r3
    bcc CopyDataInit
    ldr r2, =_sbss
    b LoopFillZerobss

  /* 3. Clear .bss section (Zero init) */
FillZerobss:
    movs r3, #0
    str r3, [r2], #4

LoopFillZerobss:
    ldr r3, = _ebss
    cmp r2, r3
    bcc FillZerobss

    /* 4. Run static constructors  */
    bl __libc_init_array

    /* 5. Enter main  */
    bl main

    /* 6. Loop forever if returning from main */
LoopForever:
    b LoopForever


.size   Reset_Handler, .-Reset_Handler

/**
**===========================================================================
**  Program - Default_Handler
**  Abstract: This code gets called when the processor receives an
**    unexpected interrupt.
**===========================================================================
*/
    .section    .text.Default_Handler,"ax",%progbits
Default_Handler:
InfiniteLoop:
    b   InfiniteLoop
    .size   Default_Handler, .-Default_Handler

/**
**===========================================================================
**  Reset, Exception, and Interrupt vectors
**===========================================================================
*/
    .section    .vectors,"a",%progbits
    .type   g_pfnVectors, %object
    .size   g_pfnVectors, .-g_pfnVectors


g_pfnVectors:
__Vectors:
    /* Processor exception vectors */
    .word   __stack_end__
    .word   Reset_Handler
    .word   NMI_Handler
    .word   HardFault_Handler
    .word   MemManage_Handler
    .word   BusFault_Handler
    .word   UsageFault_Handler
    .word   0
    .word   0
    .word   0
    .word   0
    .word   SVC_Handler
    .word   DebugMon_Handler
    .word   0
    .word   PendSV_Handler
    .word   SysTick_Handler

    /* Add processor interrupts here */
    .word   0

/**
**===========================================================================
** Provide weak aliases for each Exception handler to the Default_Handler.
**===========================================================================
*/
    .weak  NMI_Handler
    .thumb_set NMI_Handler,Default_Handler
  
    .weak  HardFault_Handler
    .thumb_set HardFault_Handler,Default_Handler
  
    .weak  MemManage_Handler
    .thumb_set MemManage_Handler,Default_Handler
  
    .weak  BusFault_Handler
    .thumb_set BusFault_Handler,Default_Handler

    .weak  UsageFault_Handler
    .thumb_set UsageFault_Handler,Default_Handler

    .weak  SVC_Handler
    .thumb_set SVC_Handler,Default_Handler

    .weak  DebugMon_Handler
    .thumb_set DebugMon_Handler,Default_Handler

    .weak  PendSV_Handler
    .thumb_set PendSV_Handler,Default_Handler

    .weak  SysTick_Handler
    .thumb_set SysTick_Handler,Default_Handler

.end
