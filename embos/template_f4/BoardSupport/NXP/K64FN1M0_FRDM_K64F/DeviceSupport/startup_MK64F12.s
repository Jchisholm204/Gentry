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
File    : startup_MK64F12.s
Purpose : Startup code and vector table for NXP MK64F12
--------  END-OF-HEADER  ---------------------------------------------
*/

  .syntax unified
    .cpu cortex-m4
    .fpu softvfp
    .thumb

.global  g_pfnVectors
.global Default_Handler
.global __Vectors
.global getPC

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

    /* FPU settings */
    ldr     r0, =0xE000ED88
    ldr     r1,[r0]
    orr     r1,r1,#(0xF << 20)
    str     r1,[r0]

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

getPC:
      MOV   R0,LR
      BX    LR


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
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors


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

    .word   DMA0_IRQHandler
    .word   DMA1_IRQHandler
    .word   DMA2_IRQHandler
    .word   DMA3_IRQHandler
    .word   DMA4_IRQHandler
    .word   DMA5_IRQHandler
    .word   DMA6_IRQHandler
    .word   DMA7_IRQHandler
    .word   DMA8_IRQHandler
    .word   DMA9_IRQHandler
    .word   DMA10_IRQHandler
    .word   DMA11_IRQHandler
    .word   DMA12_IRQHandler
    .word   DMA13_IRQHandler
    .word   DMA14_IRQHandler
    .word   DMA15_IRQHandler
    .word   DMA_Error_IRQHandler
    .word   0
    .word   FTFE_IRQHandler
    .word   Read_Collision_IRQHandler
    .word   LVD_LVW_IRQHandler
    .word   LLW_IRQHandler
    .word   Watchdog_IRQHandler
    .word   RNG_IRQHandler
    .word   I2C0_IRQHandler
    .word   I2C1_IRQHandler
    .word   SPI0_IRQHandler
    .word   SPI1_IRQHandler
    .word   I2S0_Tx_IRQHandler
    .word   I2S0_Rx_IRQHandler
    .word   UART0_LON_IRQHandler
    .word   UART0_RX_TX_IRQHandler
    .word   UART0_ERR_IRQHandler
    .word   UART1_RX_TX_IRQHandler
    .word   UART1_ERR_IRQHandler
    .word   UART2_RX_TX_IRQHandler
    .word   UART2_ERR_IRQHandler
    .word   UART3_RX_TX_IRQHandler
    .word   UART3_ERR_IRQHandler
    .word   ADC0_IRQHandler
    .word   CMP0_IRQHandler
    .word   CMP1_IRQHandler
    .word   FTM0_IRQHandler
    .word   FTM1_IRQHandler
    .word   FTM2_IRQHandler
    .word   CMT_IRQHandler
    .word   RTC_IRQHandler
    .word   RTC_Seconds_IRQHandler
    .word   PIT0_IRQHandler
    .word   PIT1_IRQHandler
    .word   PIT2_IRQHandler
    .word   PIT3_IRQHandler
    .word   PDB0_IRQHandler
    .word   USB0_IRQHandler
    .word   USBDCD_IRQHandler
    .word   0
    .word   DAC0_IRQHandler
    .word   0
    .word   LPTimer_IRQHandler
    .word   PORTA_IRQHandler
    .word   PORTB_IRQHandler
    .word   PORTC_IRQHandler
    .word   PORTD_IRQHandler
    .word   PORTE_IRQHandler
    .word   0
    .word   SPI2_IRQHandler
    .word   UART4_RX_TX_IRQHandler
    .word   UART4_ERR_IRQHandler
    .word   UART5_RX_TX_IRQHandler
    .word   UART5_ERR_IRQHandler
    .word   CMP2_IRQHandler
    .word   FTM3_IRQHandler
    .word   DAC1_IRQHandler
    .word   ADC1_IRQHandler
    .word   I2C2_IRQHandler
    .word   CAN0_ORed_Message_buffer_IRQHandler
    .word   CAN0_Bus_Off_IRQHandler
    .word   CAN0_Error_IRQHandler
    .word   CAN0_Tx_Warning_IRQHandler
    .word   CAN0_Rx_Warning_IRQHandler
    .word   CAN0_Wake_Up_IRQHandler
    .word   SDHC_IRQHandler
    .word   ENET_1588_Timer_IRQHandler
    .word   ENET_Transmit_IRQHandler
    .word   ENET_Receive_IRQHandler
    .word   ENET_Error_IRQHandler

/**
**===========================================================================
** Provide weak aliases for each Exception handler to the Default_Handler.
**===========================================================================
*/

    .weak   NMI_Handler
    .thumb_set NMI_Handler,Default_Handler

    .weak   HardFault_Handler
    .thumb_set HardFault_Handler,Default_Handler

    .weak   MemManage_Handler
    .thumb_set MemManage_Handler,Default_Handler

    .weak   BusFault_Handler
    .thumb_set BusFault_Handler,Default_Handler

    .weak   UsageFault_Handler
    .thumb_set UsageFault_Handler,Default_Handler

    .weak   SVC_Handler
    .thumb_set SVC_Handler,Default_Handler

    .weak   DebugMon_Handler
    .thumb_set DebugMon_Handler,Default_Handler

    .weak   PendSV_Handler
    .thumb_set PendSV_Handler,Default_Handler

    .weak   SysTick_Handler
    .thumb_set SysTick_Handler,Default_Handler

    .weak   DMA0_IRQHandler
    .thumb_set DMA0_IRQHandler,Default_Handler

    .weak   DMA1_IRQHandler
    .thumb_set DMA1_IRQHandler,Default_Handler

    .weak   DMA2_IRQHandler
    .thumb_set DMA2_IRQHandler,Default_Handler

    .weak   DMA3_IRQHandler
    .thumb_set DMA3_IRQHandler,Default_Handler

    .weak   DMA4_IRQHandler
    .thumb_set DMA4_IRQHandler,Default_Handler

    .weak   DMA5_IRQHandler
    .thumb_set DMA5_IRQHandler,Default_Handler

    .weak   DMA6_IRQHandler
    .thumb_set DMA6_IRQHandler,Default_Handler

    .weak   DMA7_IRQHandler
    .thumb_set DMA7_IRQHandler,Default_Handler

    .weak   DMA8_IRQHandler
    .thumb_set DMA8_IRQHandler,Default_Handler

    .weak   DMA9_IRQHandler
    .thumb_set DMA9_IRQHandler,Default_Handler

    .weak   DMA10_IRQHandler
    .thumb_set DMA10_IRQHandler,Default_Handler

    .weak   DMA11_IRQHandler
    .thumb_set DMA11_IRQHandler,Default_Handler

    .weak   DMA12_IRQHandler
    .thumb_set DMA12_IRQHandler,Default_Handler

    .weak   DMA13_IRQHandler
    .thumb_set DMA13_IRQHandler,Default_Handler

    .weak   DMA14_IRQHandler
    .thumb_set DMA14_IRQHandler,Default_Handler

    .weak   DMA15_IRQHandler
    .thumb_set DMA15_IRQHandler,Default_Handler

    .weak   DMA_Error_IRQHandler
    .thumb_set DMA_Error_IRQHandler,Default_Handler


    .weak   FTFE_IRQHandler
    .thumb_set FTFE_IRQHandler,Default_Handler

    .weak   Read_Collision_IRQHandler
    .thumb_set Read_Collision_IRQHandler,Default_Handler

    .weak   LVD_LVW_IRQHandler
    .thumb_set LVD_LVW_IRQHandler,Default_Handler

    .weak   LLW_IRQHandler
    .thumb_set LLW_IRQHandler,Default_Handler

    .weak   Watchdog_IRQHandler
    .thumb_set Watchdog_IRQHandler,Default_Handler

    .weak   RNG_IRQHandler
    .thumb_set RNG_IRQHandler,Default_Handler

    .weak   I2C0_IRQHandler
    .thumb_set I2C0_IRQHandler,Default_Handler

    .weak   I2C1_IRQHandler
    .thumb_set I2C1_IRQHandler,Default_Handler

    .weak   SPI0_IRQHandler
    .thumb_set SPI0_IRQHandler,Default_Handler

    .weak   SPI1_IRQHandler
    .thumb_set SPI1_IRQHandler,Default_Handler

    .weak   I2S0_Tx_IRQHandler
    .thumb_set I2S0_Tx_IRQHandler,Default_Handler

    .weak   I2S0_Rx_IRQHandler
    .thumb_set I2S0_Rx_IRQHandler,Default_Handler

    .weak   UART0_LON_IRQHandler
    .thumb_set UART0_LON_IRQHandler,Default_Handler

    .weak   UART0_RX_TX_IRQHandler
    .thumb_set UART0_RX_TX_IRQHandler,Default_Handler

    .weak   UART0_ERR_IRQHandler
    .thumb_set UART0_ERR_IRQHandler,Default_Handler

    .weak   UART1_RX_TX_IRQHandler
    .thumb_set UART1_RX_TX_IRQHandler,Default_Handler

    .weak   UART1_ERR_IRQHandler
    .thumb_set UART1_ERR_IRQHandler,Default_Handler

    .weak   UART2_RX_TX_IRQHandler
    .thumb_set UART2_RX_TX_IRQHandler,Default_Handler

    .weak   UART2_ERR_IRQHandler
    .thumb_set UART2_ERR_IRQHandler,Default_Handler

    .weak   UART3_RX_TX_IRQHandler
    .thumb_set UART3_RX_TX_IRQHandler,Default_Handler

    .weak   UART3_ERR_IRQHandler
    .thumb_set UART3_ERR_IRQHandler,Default_Handler

    .weak   ADC0_IRQHandler
    .thumb_set ADC0_IRQHandler,Default_Handler

    .weak   CMP0_IRQHandler
    .thumb_set CMP0_IRQHandler,Default_Handler

    .weak   CMP1_IRQHandler
    .thumb_set CMP1_IRQHandler,Default_Handler

    .weak   FTM0_IRQHandler
    .thumb_set FTM0_IRQHandler,Default_Handler

    .weak   FTM1_IRQHandler
    .thumb_set FTM1_IRQHandler,Default_Handler

    .weak   FTM2_IRQHandler
    .thumb_set FTM2_IRQHandler,Default_Handler

    .weak   CMT_IRQHandler
    .thumb_set CMT_IRQHandler,Default_Handler

    .weak   RTC_IRQHandler
    .thumb_set RTC_IRQHandler,Default_Handler

    .weak   RTC_Seconds_IRQHandler
    .thumb_set RTC_Seconds_IRQHandler,Default_Handler

    .weak   PIT0_IRQHandler
    .thumb_set PIT0_IRQHandler,Default_Handler

    .weak   PIT1_IRQHandler
    .thumb_set PIT1_IRQHandler,Default_Handler

    .weak   PIT2_IRQHandler
    .thumb_set PIT2_IRQHandler,Default_Handler

    .weak   PIT3_IRQHandler
    .thumb_set PIT3_IRQHandler,Default_Handler

    .weak   PDB0_IRQHandler
    .thumb_set PDB0_IRQHandler,Default_Handler

    .weak   USB0_IRQHandler
    .thumb_set USB0_IRQHandler,Default_Handler

    .weak   USBDCD_IRQHandler
    .thumb_set USBDCD_IRQHandler,Default_Handler


    .weak   DAC0_IRQHandler
    .thumb_set DAC0_IRQHandler,Default_Handler


    .weak   LPTimer_IRQHandler
    .thumb_set LPTimer_IRQHandler,Default_Handler

    .weak   PORTA_IRQHandler
    .thumb_set PORTA_IRQHandler,Default_Handler

    .weak   PORTB_IRQHandler
    .thumb_set PORTB_IRQHandler,Default_Handler

    .weak   PORTC_IRQHandler
    .thumb_set PORTC_IRQHandler,Default_Handler

    .weak   PORTD_IRQHandler
    .thumb_set PORTD_IRQHandler,Default_Handler

    .weak   PORTE_IRQHandler
    .thumb_set PORTE_IRQHandler,Default_Handler


    .weak   SPI2_IRQHandler
    .thumb_set SPI2_IRQHandler,Default_Handler

    .weak   UART4_RX_TX_IRQHandler
    .thumb_set UART4_RX_TX_IRQHandler,Default_Handler

    .weak   UART4_ERR_IRQHandler
    .thumb_set UART4_ERR_IRQHandler,Default_Handler

    .weak   UART5_RX_TX_IRQHandler
    .thumb_set UART5_RX_TX_IRQHandler,Default_Handler

    .weak   UART5_ERR_IRQHandler
    .thumb_set UART5_ERR_IRQHandler,Default_Handler

    .weak   CMP2_IRQHandler
    .thumb_set CMP2_IRQHandler,Default_Handler

    .weak   FTM3_IRQHandler
    .thumb_set FTM3_IRQHandler,Default_Handler

    .weak   DAC1_IRQHandler
    .thumb_set DAC1_IRQHandler,Default_Handler

    .weak   ADC1_IRQHandler
    .thumb_set ADC1_IRQHandler,Default_Handler

    .weak   I2C2_IRQHandler
    .thumb_set I2C2_IRQHandler,Default_Handler

    .weak   CAN0_ORed_Message_buffer_IRQHandler
    .thumb_set CAN0_ORed_Message_buffer_IRQHandler,Default_Handler

    .weak   CAN0_Bus_Off_IRQHandler
    .thumb_set CAN0_Bus_Off_IRQHandler,Default_Handler

    .weak   CAN0_Error_IRQHandler
    .thumb_set CAN0_Error_IRQHandler,Default_Handler

    .weak   CAN0_Tx_Warning_IRQHandler
    .thumb_set CAN0_Tx_Warning_IRQHandler,Default_Handler

    .weak   CAN0_Rx_Warning_IRQHandler
    .thumb_set CAN0_Rx_Warning_IRQHandler,Default_Handler

    .weak   CAN0_Wake_Up_IRQHandler
    .thumb_set CAN0_Wake_Up_IRQHandler,Default_Handler

    .weak   SDHC_IRQHandler
    .thumb_set SDHC_IRQHandler,Default_Handler

    .weak   ENET_1588_Timer_IRQHandler
    .thumb_set ENET_1588_Timer_IRQHandler,Default_Handler

    .weak   ENET_Transmit_IRQHandler
    .thumb_set ENET_Transmit_IRQHandler,Default_Handler

    .weak   ENET_Receive_IRQHandler
    .thumb_set ENET_Receive_IRQHandler,Default_Handler

    .weak   ENET_Error_IRQHandler
    .thumb_set ENET_Error_IRQHandler,Default_Handler
