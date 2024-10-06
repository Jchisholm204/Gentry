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
File    : startup_LPC43xx.s
Purpose : Startup code and vector table for NXP LPC43xx
--------  END-OF-HEADER  ---------------------------------------------
*/

  .syntax unified
  .cpu cortex-m4
  .fpu softvfp
  .thumb

.global Reset_Handler
.global Default_Handler
.global __Vectors
.global getPC

/* Linker script definitions */
/* start address for the initialization values of the .data section */
    .word     _sidata
/* start address for the .data section */
    .word     _sdata
/* end address for the .data section */
    .word     _edata
/* start address for the .bss section */
    .word     _sbss
/* end address for the .bss section */
    .word     _ebss

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
    .section .text.Reset_Handler, "x", %progbits
    .type Reset_Handler, %function
Reset_Handler:
    /* set stack pointer */
    ldr     sp, =__stack_end__

    /* FPU settings */
    ldr     r0, =0xE000ED88
    ldr     r1,[r0]
    orr     r1,r1,#(0xF << 20)
    str     r1,[r0]

    /* 1. Call system initialzation routine */
    bl      SystemInit

    /* 2. copy .data section (Copy from ROM to RAM) */
    movs    r1, #0
    b       LoopCopyDataInit

CopyDataInit:
    ldr     r3, =_sidata
    ldr     r3, [r3, r1]
    str     r3, [r0, r1]
    adds    r1, r1, #4

LoopCopyDataInit:
    ldr     r0, =_sdata
    ldr     r3, =_edata
    adds    r2, r0, r1
    cmp     r2, r3
    bcc     CopyDataInit
    ldr     r2, =_sbss
    b       LoopFillZerobss

    /* 3. Clear .bss section (Zero init) */
FillZerobss:
    movs    r3, #0
    str     r3, [r2], #4

LoopFillZerobss:
    ldr     r3, = _ebss
    cmp     r2, r3
    bcc     FillZerobss

    /* 4. Run static constructors  */
    bl      __libc_init_array

    /* 5. Enter main  */
    bl      main

    /* 6. Loop forever if returning from main */
LoopForever:
    b       LoopForever

getPC:
    mov     r0,lr
    bx      lr

    .size Reset_Handler, .-Reset_Handler

/**
**===========================================================================
**  Program - Default_Handler
**  Abstract: This code gets called when the processor receives an
**    unexpected interrupt.
**===========================================================================
*/
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
    b       Default_Handler

    .size Default_Handler, .-Default_Handler

/**
**===========================================================================
**  Reset, Exception, and Interrupt vectors
**===========================================================================
*/
    .section .vectors,"a",%progbits
    .type __Vectors, %object
    .size __Vectors, .-__Vectors

__Vectors:
    .word   __stack_end__               /* 0 - Stack pointer             */
    .word   Reset_Handler               /* 1 - Reset                     */
    .word   NMI_Handler                 /* 2 - NMI                       */
    .word   HardFault_Handler           /* 3 - Hard fault                */
    .word   MemManage_Handler           /* 4 - Memory management fault   */
    .word   BusFault_Handler            /* 5 - Bus fault                 */
    .word   UsageFault_Handler          /* 6 - Usage fault               */
    .word   0                           /* 7 - Reserved                  */
    .word   0                           /* 8 - Reserved                  */
    .word   0                           /* 9 - Reserved                  */
    .word   0                           /* 10 - Reserved                 */
    .word   SVC_Handler                 /* 11 - SVCall                   */
    .word   DebugMonitor_Handler        /* 12 - Reservered for Debug     */
    .word   0                           /* 13 - Reserved                 */
    .word   PendSV_Handler              /* 14 - PendSV                   */
    .word   SysTick_Handler             /* 15 - Systick                  */

    .word   DAC_IRQHandler              /* 16 D/A Converter              */
    .word   M0CORE_IRQHandler           /* 17 - M0 Core                  */
    .word   DMA_IRQHandler              /* 18 General Purpose DMA        */
    .word   EZH_IRQHandler              /* 19 - EZH/EDM                  */
    .word   FLASH_EEPROM_IRQHandler     /* 20 - Reserved for Typhoon     */
    .word   ETH_IRQHandler              /* 21 Ethernet                   */
    .word   SDIO_IRQHandler             /* 22 SD/MMC                     */
    .word   LCD_IRQHandler              /* 23 LCD                        */
    .word   USB0_IRQHandler             /* 24 USB0                       */
    .word   USB1_IRQHandler             /* 25 USB1                       */
    .word   SCT_IRQHandler              /* 26 State Configurable Timer   */
    .word   RIT_IRQHandler              /* 27 Repetitive Interrupt Timer */
    .word   TIMER0_IRQHandler           /* 28 Timer0                     */
    .word   TIMER1_IRQHandler           /* 29 Timer1                     */
    .word   TIMER2_IRQHandler           /* 30 Timer2                     */
    .word   TIMER3_IRQHandler           /* 31 Timer3                     */
    .word   MCPWM_IRQHandler            /* 32 Motor Control PWM          */
    .word   ADC0_IRQHandler             /* 33 A/D Converter 0            */
    .word   I2C0_IRQHandler             /* 34 I2C0                       */
    .word   I2C1_IRQHandler             /* 35 I2C1                       */
    .word   SPI_IRQHandler              /* 36 - SPI                      */
    .word   ADC1_IRQHandler             /* 37 A/D Converter 1            */
    .word   SSP0_IRQHandler             /* 38 SSP0                       */
    .word   SSP1_IRQHandler             /* 39 SSP1                       */
    .word   UART0_IRQHandler            /* 40 UART0                      */
    .word   UART1_IRQHandler            /* 41 UART1                      */
    .word   UART2_IRQHandler            /* 42 UART2                      */
    .word   UART3_IRQHandler            /* 43 UART3                      */
    .word   I2S0_IRQHandler             /* 44 I2S0                       */
    .word   I2S1_IRQHandler             /* 45 I2S1                       */
    .word   SPIFI_IRQHandler            /* 46 SPI Flash Interface        */
    .word   SGPIO_IRQHandler            /* 47 SGPIO                      */
    .word   GPIO0_IRQHandler            /* 48 GPIO0                      */
    .word   GPIO1_IRQHandler            /* 49 GPIO1                      */
    .word   GPIO2_IRQHandler            /* 50 GPIO2                      */
    .word   GPIO3_IRQHandler            /* 51 GPIO3                      */
    .word   GPIO4_IRQHandler            /* 52 GPIO4                      */
    .word   GPIO5_IRQHandler            /* 53 GPIO5                      */
    .word   GPIO6_IRQHandler            /* 54 GPIO6                      */
    .word   GPIO7_IRQHandler            /* 55 GPIO7                      */
    .word   GINT0_IRQHandler            /* 56 GINT0                      */
    .word   GINT1_IRQHandler            /* 57 GINT1                      */
    .word   EVRT_IRQHandler             /* 58 Event Router               */
    .word   CAN1_IRQHandler             /* 59 C_CAN1                     */
    .word   0                           /* 60 Reserved                   */
    .word   VADC_IRQHandler             /* 61 - VADC                     */
    .word   ATIMER_IRQHandler           /* 62 ATIMER                     */
    .word   RTC_IRQHandler              /* 63 RTC                        */
    .word   0                           /* 64 Reserved                   */
    .word   WDT_IRQHandler              /* 65 WDT                        */
    .word   M0s_IRQHandler              /* 66 - M0s                      */
    .word   CAN0_IRQHandler             /* 67 C_CAN0                     */
    .word   QEI_IRQHandler              /* 68 QEI                        */

/**
**===========================================================================
**  Weak interrupt handlers redirected to Default_Handler. These can be
**  overridden in user code.
**===========================================================================
*/
    .weak NMI_Handler
    .weak HardFault_Handler
    .weak MemManage_Handler
    .weak BusFault_Handler
    .weak UsageFault_Handler
    .weak SVC_Handler
    .weak DebugMonitor_Handler
    .weak PendSV_Handler
    .weak SysTick_Handler
    .weak DAC_IRQHandler
    .weak M0CORE_IRQHandler
    .weak DMA_IRQHandler
    .weak EZH_IRQHandler
    .weak FLASH_EEPROM_IRQHandler
    .weak ETH_IRQHandler
    .weak SDIO_IRQHandler
    .weak LCD_IRQHandler
    .weak USB0_IRQHandler
    .weak USB1_IRQHandler
    .weak SCT_IRQHandler
    .weak RIT_IRQHandler
    .weak TIMER0_IRQHandler
    .weak TIMER1_IRQHandler
    .weak TIMER2_IRQHandler
    .weak TIMER3_IRQHandler
    .weak MCPWM_IRQHandler
    .weak ADC0_IRQHandler
    .weak I2C0_IRQHandler
    .weak I2C1_IRQHandler
    .weak SPI_IRQHandler
    .weak ADC1_IRQHandler
    .weak SSP0_IRQHandler
    .weak SSP1_IRQHandler
    .weak UART0_IRQHandler
    .weak UART1_IRQHandler
    .weak UART2_IRQHandler
    .weak UART3_IRQHandler
    .weak I2S0_IRQHandler
    .weak I2S1_IRQHandler
    .weak SPIFI_IRQHandler
    .weak SGPIO_IRQHandler
    .weak GPIO0_IRQHandler
    .weak GPIO1_IRQHandler
    .weak GPIO2_IRQHandler
    .weak GPIO3_IRQHandler
    .weak GPIO4_IRQHandler
    .weak GPIO5_IRQHandler
    .weak GPIO6_IRQHandler
    .weak GPIO7_IRQHandler
    .weak GINT0_IRQHandler
    .weak GINT1_IRQHandler
    .weak EVRT_IRQHandler
    .weak CAN1_IRQHandler
    .weak VADC_IRQHandler
    .weak ATIMER_IRQHandler
    .weak RTC_IRQHandler
    .weak WDT_IRQHandler
    .weak M0s_IRQHandler
    .weak CAN0_IRQHandler
    .weak QEI_IRQHandler

    .thumb_set NMI_Handler, Default_Handler
    .thumb_set HardFault_Handler, Default_Handler
    .thumb_set MemManage_Handler, Default_Handler
    .thumb_set BusFault_Handler, Default_Handler
    .thumb_set UsageFault_Handler, Default_Handler
    .thumb_set SVC_Handler, Default_Handler
    .thumb_set DebugMonitor_Handler, Default_Handler
    .thumb_set PendSV_Handler, Default_Handler
    .thumb_set SysTick_Handler, Default_Handler
    .thumb_set DAC_IRQHandler, Default_Handler
    .thumb_set M0CORE_IRQHandler, Default_Handler
    .thumb_set DMA_IRQHandler, Default_Handler
    .thumb_set EZH_IRQHandler, Default_Handler
    .thumb_set FLASH_EEPROM_IRQHandler, Default_Handler
    .thumb_set ETH_IRQHandler, Default_Handler
    .thumb_set SDIO_IRQHandler, Default_Handler
    .thumb_set LCD_IRQHandler, Default_Handler
    .thumb_set USB0_IRQHandler, Default_Handler
    .thumb_set USB1_IRQHandler, Default_Handler
    .thumb_set SCT_IRQHandler, Default_Handler
    .thumb_set RIT_IRQHandler, Default_Handler
    .thumb_set TIMER0_IRQHandler, Default_Handler
    .thumb_set TIMER1_IRQHandler, Default_Handler
    .thumb_set TIMER2_IRQHandler, Default_Handler
    .thumb_set TIMER3_IRQHandler, Default_Handler
    .thumb_set MCPWM_IRQHandler, Default_Handler
    .thumb_set ADC0_IRQHandler, Default_Handler
    .thumb_set I2C0_IRQHandler, Default_Handler
    .thumb_set I2C1_IRQHandler, Default_Handler
    .thumb_set SPI_IRQHandler, Default_Handler
    .thumb_set ADC1_IRQHandler, Default_Handler
    .thumb_set SSP0_IRQHandler, Default_Handler
    .thumb_set SSP1_IRQHandler, Default_Handler
    .thumb_set UART0_IRQHandler, Default_Handler
    .thumb_set UART1_IRQHandler, Default_Handler
    .thumb_set UART2_IRQHandler, Default_Handler
    .thumb_set UART3_IRQHandler, Default_Handler
    .thumb_set I2S0_IRQHandler, Default_Handler
    .thumb_set I2S1_IRQHandler, Default_Handler
    .thumb_set SPIFI_IRQHandler, Default_Handler
    .thumb_set SGPIO_IRQHandler, Default_Handler
    .thumb_set GPIO0_IRQHandler, Default_Handler
    .thumb_set GPIO1_IRQHandler, Default_Handler
    .thumb_set GPIO2_IRQHandler, Default_Handler
    .thumb_set GPIO3_IRQHandler, Default_Handler
    .thumb_set GPIO4_IRQHandler, Default_Handler
    .thumb_set GPIO5_IRQHandler, Default_Handler
    .thumb_set GPIO6_IRQHandler, Default_Handler
    .thumb_set GPIO7_IRQHandler, Default_Handler
    .thumb_set GINT0_IRQHandler, Default_Handler
    .thumb_set GINT1_IRQHandler, Default_Handler
    .thumb_set EVRT_IRQHandler, Default_Handler
    .thumb_set CAN1_IRQHandler, Default_Handler
    .thumb_set VADC_IRQHandler, Default_Handler
    .thumb_set ATIMER_IRQHandler, Default_Handler
    .thumb_set RTC_IRQHandler, Default_Handler
    .thumb_set WDT_IRQHandler, Default_Handler
    .thumb_set M0s_IRQHandler, Default_Handler
    .thumb_set CAN0_IRQHandler, Default_Handler
    .thumb_set QEI_IRQHandler, Default_Handler
