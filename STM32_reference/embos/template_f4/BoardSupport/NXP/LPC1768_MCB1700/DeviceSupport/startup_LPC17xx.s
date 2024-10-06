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
File    : startup_LPC17xx.s
Purpose : Startup code and vector table for NXP LPC17xx
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
    .word   WDT_IRQHandler              /* 16: Watchdog Timer               */
    .word   TIMER0_IRQHandler           /* 17: Timer0                       */
    .word   TIMER1_IRQHandler           /* 18: Timer1                       */
    .word   TIMER2_IRQHandler           /* 19: Timer2                       */
    .word   TIMER3_IRQHandler           /* 20: Timer3                       */
    .word   UART0_IRQHandler            /* 21: UART0                        */
    .word   UART1_IRQHandler            /* 22: UART1                        */
    .word   UART2_IRQHandler            /* 23: UART2                        */
    .word   UART3_IRQHandler            /* 24: UART3                        */
    .word   PWM1_IRQHandler             /* 25: PWM1                         */
    .word   I2C0_IRQHandler             /* 26: I2C0                         */
    .word   I2C1_IRQHandler             /* 27: I2C1                         */
    .word   I2C2_IRQHandler             /* 28: I2C2                         */
    .word   SPI_IRQHandler              /* 29: SPI                          */
    .word   SSP0_IRQHandler             /* 30: SSP0                         */
    .word   SSP1_IRQHandler             /* 31: SSP1                         */
    .word   PLL0_IRQHandler             /* 32: PLL0 Lock (Main PLL)         */
    .word   RTC_IRQHandler              /* 33: Real Time Clock              */
    .word   EINT0_IRQHandler            /* 34: External Interrupt 0         */
    .word   EINT1_IRQHandler            /* 35: External Interrupt 1         */
    .word   EINT2_IRQHandler            /* 36: External Interrupt 2         */
    .word   EINT3_IRQHandler            /* 37: External Interrupt 3         */
    .word   ADC_IRQHandler              /* 38: A/D Converter                */
    .word   BOD_IRQHandler              /* 39: Brown-Out Detect             */
    .word   USB_IRQHandler              /* 40: USB                          */
    .word   CAN_IRQHandler              /* 41: CAN                          */
    .word   DMA_IRQHandler              /* 42: General Purpose DMA          */
    .word   I2S_IRQHandler              /* 43: I2S                          */
    .word   ENET_IRQHandler             /* 44: Ethernet                     */
    .word   RIT_IRQHandler              /* 45: Repetitive Interrupt Timer   */
    .word   MCPWM_IRQHandler            /* 46: Motor Control PWM            */
    .word   QEI_IRQHandler              /* 47: Quadrature Encoder Interface */
    .word   PLL1_IRQHandler             /* 48: PLL1 Lock (USB PLL)          */
    .word   USBActivity_IRQHandler      /* 49: USB Activity                 */
    .word   CANActivity_IRQHandler      /* 50: CAN Activity                 */

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


    .macro  IRQ handler
    .weak   \handler
    .set    \handler, Default_Handler
    .endm

    IRQ     WDT_IRQHandler
    IRQ     TIMER0_IRQHandler
    IRQ     TIMER1_IRQHandler
    IRQ     TIMER2_IRQHandler
    IRQ     TIMER3_IRQHandler
    IRQ     UART0_IRQHandler
    IRQ     UART1_IRQHandler
    IRQ     UART2_IRQHandler
    IRQ     UART3_IRQHandler
    IRQ     PWM1_IRQHandler
    IRQ     I2C0_IRQHandler
    IRQ     I2C1_IRQHandler
    IRQ     I2C2_IRQHandler
    IRQ     SPI_IRQHandler
    IRQ     SSP0_IRQHandler
    IRQ     SSP1_IRQHandler
    IRQ     PLL0_IRQHandler
    IRQ     RTC_IRQHandler
    IRQ     EINT0_IRQHandler
    IRQ     EINT1_IRQHandler
    IRQ     EINT2_IRQHandler
    IRQ     EINT3_IRQHandler
    IRQ     ADC_IRQHandler
    IRQ     BOD_IRQHandler
    IRQ     USB_IRQHandler
    IRQ     CAN_IRQHandler
    IRQ     DMA_IRQHandler
    IRQ     I2S_IRQHandler
    IRQ     ENET_IRQHandler
    IRQ     RIT_IRQHandler
    IRQ     MCPWM_IRQHandler
    IRQ     QEI_IRQHandler
    IRQ     PLL1_IRQHandler
    IRQ     USBActivity_IRQHandler
    IRQ     CANActivity_IRQHandler

.end
