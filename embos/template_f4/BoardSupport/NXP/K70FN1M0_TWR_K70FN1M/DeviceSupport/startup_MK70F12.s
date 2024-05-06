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
File    : startup_MK70F12.s
Purpose : CMSIS Startup code and vector table for Freescale K70
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

    /* Peripheral interrupts */
    .word   DMA0_DMA16_IRQHandler  /* DMA channel 0/16 transfer complete interrupt */
    .word   DMA1_DMA17_IRQHandler  /* DMA channel 1/17 transfer complete interrupt */
    .word   DMA2_DMA18_IRQHandler  /* DMA channel 2/18 transfer complete interrupt */
    .word   DMA3_DMA19_IRQHandler  /* DMA channel 3/19 transfer complete interrupt */
    .word   DMA4_DMA20_IRQHandler  /* DMA channel 4/20 transfer complete interrupt */
    .word   DMA5_DMA21_IRQHandler  /* DMA channel 5/21 transfer complete interrupt */
    .word   DMA6_DMA22_IRQHandler  /* DMA channel 6/22 transfer complete interrupt */
    .word   DMA7_DMA23_IRQHandler  /* DMA channel 7/23 transfer complete interrupt */
    .word   DMA8_DMA24_IRQHandler  /* DMA channel 8/24 transfer complete interrupt */
    .word   DMA9_DMA25_IRQHandler  /* DMA channel 9/25 transfer complete interrupt */
    .word   DMA10_DMA26_IRQHandler  /* DMA channel 10/26 transfer complete interrupt */
    .word   DMA11_DMA27_IRQHandler  /* DMA channel 11/27 transfer complete interrupt */
    .word   DMA12_DMA28_IRQHandler  /* DMA channel 12/28 transfer complete interrupt */
    .word   DMA13_DMA29_IRQHandler  /* DMA channel 13/29 transfer complete interrupt */
    .word   DMA14_DMA30_IRQHandler  /* DMA channel 14/30 transfer complete interrupt */
    .word   DMA15_DMA31_IRQHandler  /* DMA channel 15/31 transfer complete interrupt */
    .word   DMA_Error_IRQHandler  /* DMA error interrupt */
    .word   MCM_IRQHandler  /* Normal interrupt */
    .word   FTFE_IRQHandler  /* FTFE interrupt */
    .word   Read_Collision_IRQHandler  /* Read collision interrupt */
    .word   LVD_LVW_IRQHandler  /* Low Voltage Detect, Low Voltage Warning */
    .word   LLW_IRQHandler  /* Low Leakage Wakeup */
    .word   Watchdog_IRQHandler  /* WDOG interrupt */
    .word   RNG_IRQHandler  /* RNGA interrupt */
    .word   I2C0_IRQHandler  /* I2C0 interrupt */
    .word   I2C1_IRQHandler  /* I2C1 interrupt */
    .word   SPI0_IRQHandler  /* SPI0 interrupt */
    .word   SPI1_IRQHandler  /* SPI1 interrupt */
    .word   SPI2_IRQHandler  /* SPI2 interrupt */
    .word   CAN0_ORed_Message_buffer_IRQHandler  /* CAN0 OR'd message buffers interrupt */
    .word   CAN0_Bus_Off_IRQHandler  /* CAN0 bus off interrupt */
    .word   CAN0_Error_IRQHandler  /* CAN0 error interrupt */
    .word   CAN0_Tx_Warning_IRQHandler  /* CAN0 Tx warning interrupt */
    .word   CAN0_Rx_Warning_IRQHandler  /* CAN0 Rx warning interrupt */
    .word   CAN0_Wake_Up_IRQHandler  /* CAN0 wake up interrupt */
    .word   I2S0_Tx_IRQHandler  /* I2S0 transmit interrupt */
    .word   I2S0_Rx_IRQHandler  /* I2S0 receive interrupt */
    .word   CAN1_ORed_Message_buffer_IRQHandler  /* CAN1 OR'd message buffers interrupt */
    .word   CAN1_Bus_Off_IRQHandler  /* CAN1 bus off interrupt */
    .word   CAN1_Error_IRQHandler  /* CAN1 error interrupt */
    .word   CAN1_Tx_Warning_IRQHandler  /* CAN1 Tx warning interrupt */
    .word   CAN1_Rx_Warning_IRQHandler  /* CAN1 Rx warning interrupt */
    .word   CAN1_Wake_Up_IRQHandler  /* CAN1 wake up interrupt */
    .word   Reserved59_IRQHandler  /* Reserved interrupt 59 */
    .word   UART0_LON_IRQHandler  /* UART0 LON interrupt */
    .word   UART0_RX_TX_IRQHandler  /* UART0 receive/transmit interrupt */
    .word   UART0_ERR_IRQHandler  /* UART0 error interrupt */
    .word   UART1_RX_TX_IRQHandler  /* UART1 receive/transmit interrupt */
    .word   UART1_ERR_IRQHandler  /* UART1 error interrupt */
    .word   UART2_RX_TX_IRQHandler  /* UART2 receive/transmit interrupt */
    .word   UART2_ERR_IRQHandler  /* UART2 error interrupt */
    .word   UART3_RX_TX_IRQHandler  /* UART3 receive/transmit interrupt */
    .word   UART3_ERR_IRQHandler  /* UART3 error interrupt */
    .word   UART4_RX_TX_IRQHandler  /* UART4 receive/transmit interrupt */
    .word   UART4_ERR_IRQHandler  /* UART4 error interrupt */
    .word   UART5_RX_TX_IRQHandler  /* UART5 receive/transmit interrupt */
    .word   UART5_ERR_IRQHandler  /* UART5 error interrupt */
    .word   ADC0_IRQHandler  /* ADC0 interrupt */
    .word   ADC1_IRQHandler  /* ADC1 interrupt */
    .word   CMP0_IRQHandler  /* CMP0 interrupt */
    .word   CMP1_IRQHandler  /* CMP1 interrupt */
    .word   CMP2_IRQHandler  /* CMP2 interrupt */
    .word   FTM0_IRQHandler  /* FTM0 fault, overflow and channels interrupt */
    .word   FTM1_IRQHandler  /* FTM1 fault, overflow and channels interrupt */
    .word   FTM2_IRQHandler  /* FTM2 fault, overflow and channels interrupt */
    .word   CMT_IRQHandler  /* CMT interrupt */
    .word   RTC_IRQHandler  /* RTC interrupt */
    .word   RTC_Seconds_IRQHandler  /* RTC seconds interrupt */
    .word   PIT0_IRQHandler  /* PIT timer channel 0 interrupt */
    .word   PIT1_IRQHandler  /* PIT timer channel 1 interrupt */
    .word   PIT2_IRQHandler  /* PIT timer channel 2 interrupt */
    .word   PIT3_IRQHandler  /* PIT timer channel 3 interrupt */
    .word   PDB0_IRQHandler  /* PDB0 interrupt */
    .word   USB0_IRQHandler  /* USB0 interrupt */
    .word   USBDCD_IRQHandler  /* USBDCD interrupt */
    .word   ENET_1588_Timer_IRQHandler  /* Ethernet MAC IEEE 1588 timer interrupt */
    .word   ENET_Transmit_IRQHandler  /* Ethernet MAC transmit interrupt */
    .word   ENET_Receive_IRQHandler  /* Ethernet MAC receive interrupt */
    .word   ENET_Error_IRQHandler  /* Ethernet MAC error and miscelaneous interrupt */
    .word   Reserved95_IRQHandler  /* Reserved interrupt 95 */
    .word   SDHC_IRQHandler  /* SDHC interrupt */
    .word   DAC0_IRQHandler  /* DAC0 interrupt */
    .word   DAC1_IRQHandler  /* DAC1 interrupt */
    .word   TSI0_IRQHandler  /* TSI0 interrupt */
    .word   MCG_IRQHandler  /* MCG interrupt */
    .word   LPTimer_IRQHandler  /* LPTimer interrupt */
    .word   Reserved102_IRQHandler  /* Reserved interrupt 102 */
    .word   PORTA_IRQHandler  /* Port A interrupt */
    .word   PORTB_IRQHandler  /* Port B interrupt */
    .word   PORTC_IRQHandler  /* Port C interrupt */
    .word   PORTD_IRQHandler  /* Port D interrupt */
    .word   PORTE_IRQHandler  /* Port E interrupt */
    .word   PORTF_IRQHandler  /* Port F interrupt */
    .word   DDR_IRQHandler  /* DDR interrupt */
    .word   SWI_IRQHandler  /* Software interrupt */
    .word   NFC_IRQHandler  /* NAND flash controller interrupt */
    .word   USBHS_IRQHandler  /* USB high speed OTG interrupt */
    .word   LCD_IRQHandler  /* Graphical LCD interrupt */
    .word   CMP3_IRQHandler  /* CMP3 interrupt */
    .word   Tamper_IRQHandler  /* Tamper detect interrupt */
    .word   Reserved116_IRQHandler  /* Reserved interrupt 116 */
    .word   FTM3_IRQHandler  /* FTM3 fault, overflow and channels interrupt */
    .word   ADC2_IRQHandler  /* ADC2 interrupt */
    .word   ADC3_IRQHandler  /* ADC3 interrupt */
    .word   I2S1_Tx_IRQHandler  /* I2S1 transmit interrupt */
    .word   I2S1_Rx_IRQHandler  /* I2S1 receive interrupt */


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

    .weak  DMA0_DMA16_IRQHandler
    .thumb_set DMA0_DMA16_IRQHandler,Default_Handler

    .weak  DMA1_DMA17_IRQHandler
    .thumb_set DMA1_DMA17_IRQHandler,Default_Handler

    .weak  DMA2_DMA18_IRQHandler
    .thumb_set DMA2_DMA18_IRQHandler,Default_Handler

    .weak  DMA3_DMA19_IRQHandler
    .thumb_set DMA3_DMA19_IRQHandler,Default_Handler

    .weak  DMA4_DMA20_IRQHandler
    .thumb_set DMA4_DMA20_IRQHandler,Default_Handler

    .weak  DMA5_DMA21_IRQHandler
    .thumb_set DMA5_DMA21_IRQHandler,Default_Handler

    .weak  DMA6_DMA22_IRQHandler
    .thumb_set DMA6_DMA22_IRQHandler,Default_Handler

    .weak  DMA7_DMA23_IRQHandler
    .thumb_set DMA7_DMA23_IRQHandler,Default_Handler

    .weak  DMA8_DMA24_IRQHandler
    .thumb_set DMA8_DMA24_IRQHandler,Default_Handler

    .weak  DMA9_DMA25_IRQHandler
    .thumb_set DMA9_DMA25_IRQHandler,Default_Handler

    .weak  DMA10_DMA26_IRQHandler
    .thumb_set DMA10_DMA26_IRQHandler,Default_Handler

    .weak  DMA11_DMA27_IRQHandler
    .thumb_set DMA11_DMA27_IRQHandler,Default_Handler

    .weak  DMA12_DMA28_IRQHandler
    .thumb_set DMA12_DMA28_IRQHandler,Default_Handler

    .weak  DMA13_DMA29_IRQHandler
    .thumb_set DMA13_DMA29_IRQHandler,Default_Handler

    .weak  DMA14_DMA30_IRQHandler
    .thumb_set DMA14_DMA30_IRQHandler,Default_Handler

    .weak  DMA15_DMA31_IRQHandler
    .thumb_set DMA15_DMA31_IRQHandler,Default_Handler

    .weak  DMA_Error_IRQHandler
    .thumb_set DMA_Error_IRQHandler,Default_Handler

    .weak  MCM_IRQHandler
    .thumb_set MCM_IRQHandler,Default_Handler

    .weak  FTFE_IRQHandler
    .thumb_set FTFE_IRQHandler,Default_Handler

    .weak  Read_Collision_IRQHandler
    .thumb_set Read_Collision_IRQHandler,Default_Handler

    .weak  LVD_LVW_IRQHandler
    .thumb_set LVD_LVW_IRQHandler,Default_Handler

    .weak  LLW_IRQHandler
    .thumb_set LLW_IRQHandler,Default_Handler

    .weak  Watchdog_IRQHandler
    .thumb_set Watchdog_IRQHandler,Default_Handler

    .weak  RNG_IRQHandler
    .thumb_set RNG_IRQHandler,Default_Handler

    .weak  I2C0_IRQHandler
    .thumb_set I2C0_IRQHandler,Default_Handler

    .weak  I2C1_IRQHandler
    .thumb_set I2C1_IRQHandler,Default_Handler

    .weak  SPI0_IRQHandler
    .thumb_set SPI0_IRQHandler,Default_Handler

    .weak  SPI1_IRQHandler
    .thumb_set SPI1_IRQHandler,Default_Handler

    .weak  SPI2_IRQHandler
    .thumb_set SPI2_IRQHandler,Default_Handler

    .weak  CAN0_ORed_Message_buffer_IRQHandler
    .thumb_set CAN0_ORed_Message_buffer_IRQHandler,Default_Handler

    .weak  CAN0_Bus_Off_IRQHandler
    .thumb_set CAN0_Bus_Off_IRQHandler,Default_Handler

    .weak  CAN0_Error_IRQHandler
    .thumb_set CAN0_Error_IRQHandler,Default_Handler

    .weak  CAN0_Tx_Warning_IRQHandler
    .thumb_set CAN0_Tx_Warning_IRQHandler,Default_Handler

    .weak  CAN0_Rx_Warning_IRQHandler
    .thumb_set CAN0_Rx_Warning_IRQHandler,Default_Handler

    .weak  CAN0_Wake_Up_IRQHandler
    .thumb_set CAN0_Wake_Up_IRQHandler,Default_Handler

    .weak  I2S0_Tx_IRQHandler
    .thumb_set I2S0_Tx_IRQHandler,Default_Handler

    .weak  I2S0_Rx_IRQHandler
    .thumb_set I2S0_Rx_IRQHandler,Default_Handler

    .weak  CAN1_ORed_Message_buffer_IRQHandler
    .thumb_set CAN1_ORed_Message_buffer_IRQHandler,Default_Handler

    .weak  CAN1_Bus_Off_IRQHandler
    .thumb_set CAN1_Bus_Off_IRQHandler,Default_Handler

    .weak  CAN1_Error_IRQHandler
    .thumb_set CAN1_Error_IRQHandler,Default_Handler

    .weak  CAN1_Tx_Warning_IRQHandler
    .thumb_set CAN1_Tx_Warning_IRQHandler,Default_Handler

    .weak  CAN1_Rx_Warning_IRQHandler
    .thumb_set CAN1_Rx_Warning_IRQHandler,Default_Handler

    .weak  CAN1_Wake_Up_IRQHandler
    .thumb_set CAN1_Wake_Up_IRQHandler,Default_Handler

    .weak  Reserved59_IRQHandler
    .thumb_set Reserved59_IRQHandler,Default_Handler

    .weak  UART0_LON_IRQHandler
    .thumb_set UART0_LON_IRQHandler,Default_Handler

    .weak  UART0_RX_TX_IRQHandler
    .thumb_set UART0_RX_TX_IRQHandler,Default_Handler

    .weak  UART0_ERR_IRQHandler
    .thumb_set UART0_ERR_IRQHandler,Default_Handler

    .weak  UART1_RX_TX_IRQHandler
    .thumb_set UART1_RX_TX_IRQHandler,Default_Handler

    .weak  UART1_ERR_IRQHandler
    .thumb_set UART1_ERR_IRQHandler,Default_Handler

    .weak  UART2_RX_TX_IRQHandler
    .thumb_set UART2_RX_TX_IRQHandler,Default_Handler

    .weak  UART2_ERR_IRQHandler
    .thumb_set UART2_ERR_IRQHandler,Default_Handler

    .weak  UART3_RX_TX_IRQHandler
    .thumb_set UART3_RX_TX_IRQHandler,Default_Handler

    .weak  UART3_ERR_IRQHandler
    .thumb_set UART3_ERR_IRQHandler,Default_Handler

    .weak  UART4_RX_TX_IRQHandler
    .thumb_set UART4_RX_TX_IRQHandler,Default_Handler

    .weak  UART4_ERR_IRQHandler
    .thumb_set UART4_ERR_IRQHandler,Default_Handler

    .weak  UART5_RX_TX_IRQHandler
    .thumb_set UART5_RX_TX_IRQHandler,Default_Handler

    .weak  UART5_ERR_IRQHandler
    .thumb_set UART5_ERR_IRQHandler,Default_Handler

    .weak  ADC0_IRQHandler
    .thumb_set ADC0_IRQHandler,Default_Handler

    .weak  ADC1_IRQHandler
    .thumb_set ADC1_IRQHandler,Default_Handler

    .weak  CMP0_IRQHandler
    .thumb_set CMP0_IRQHandler,Default_Handler

    .weak  CMP1_IRQHandler
    .thumb_set CMP1_IRQHandler,Default_Handler

    .weak  CMP2_IRQHandler
    .thumb_set CMP2_IRQHandler,Default_Handler

    .weak  FTM0_IRQHandler
    .thumb_set FTM0_IRQHandler,Default_Handler

    .weak  FTM1_IRQHandler
    .thumb_set FTM1_IRQHandler,Default_Handler

    .weak  FTM2_IRQHandler
    .thumb_set FTM2_IRQHandler,Default_Handler

    .weak  CMT_IRQHandler
    .thumb_set CMT_IRQHandler,Default_Handler

    .weak  RTC_IRQHandler
    .thumb_set RTC_IRQHandler,Default_Handler

    .weak  RTC_Seconds_IRQHandler
    .thumb_set RTC_Seconds_IRQHandler,Default_Handler

    .weak  PIT0_IRQHandler
    .thumb_set PIT0_IRQHandler,Default_Handler

    .weak  PIT1_IRQHandler
    .thumb_set PIT1_IRQHandler,Default_Handler

    .weak  PIT2_IRQHandler
    .thumb_set PIT2_IRQHandler,Default_Handler

    .weak  PIT3_IRQHandler
    .thumb_set PIT3_IRQHandler,Default_Handler

    .weak  PDB0_IRQHandler
    .thumb_set PDB0_IRQHandler,Default_Handler

    .weak  USB0_IRQHandler
    .thumb_set USB0_IRQHandler,Default_Handler

    .weak  USBDCD_IRQHandler
    .thumb_set USBDCD_IRQHandler,Default_Handler

    .weak  ENET_1588_Timer_IRQHandler
    .thumb_set ENET_1588_Timer_IRQHandler,Default_Handler

    .weak  ENET_Transmit_IRQHandler
    .thumb_set ENET_Transmit_IRQHandler,Default_Handler

    .weak  ENET_Receive_IRQHandler
    .thumb_set ENET_Receive_IRQHandler,Default_Handler

    .weak  ENET_Error_IRQHandler
    .thumb_set ENET_Error_IRQHandler,Default_Handler

    .weak  Reserved95_IRQHandler
    .thumb_set Reserved95_IRQHandler,Default_Handler

    .weak  SDHC_IRQHandler
    .thumb_set SDHC_IRQHandler,Default_Handler

    .weak  DAC0_IRQHandler
    .thumb_set DAC0_IRQHandler,Default_Handler

    .weak  DAC1_IRQHandler
    .thumb_set DAC1_IRQHandler,Default_Handler

    .weak  TSI0_IRQHandler
    .thumb_set TSI0_IRQHandler,Default_Handler

    .weak  MCG_IRQHandler
    .thumb_set MCG_IRQHandler,Default_Handler

    .weak  LPTimer_IRQHandler
    .thumb_set LPTimer_IRQHandler,Default_Handler

    .weak  Reserved102_IRQHandler
    .thumb_set Reserved102_IRQHandler,Default_Handler

    .weak  PORTA_IRQHandler
    .thumb_set PORTA_IRQHandler,Default_Handler

    .weak  PORTB_IRQHandler
    .thumb_set PORTB_IRQHandler,Default_Handler

    .weak  PORTC_IRQHandler
    .thumb_set PORTC_IRQHandler,Default_Handler

    .weak  PORTD_IRQHandler
    .thumb_set PORTD_IRQHandler,Default_Handler

    .weak  PORTE_IRQHandler
    .thumb_set PORTE_IRQHandler,Default_Handler

    .weak  PORTF_IRQHandler
    .thumb_set PORTF_IRQHandler,Default_Handler

    .weak  DDR_IRQHandler
    .thumb_set DDR_IRQHandler,Default_Handler

    .weak  SWI_IRQHandler
    .thumb_set SWI_IRQHandler,Default_Handler

    .weak  NFC_IRQHandler
    .thumb_set NFC_IRQHandler,Default_Handler

    .weak  USBHS_IRQHandler
    .thumb_set USBHS_IRQHandler,Default_Handler

    .weak  LCD_IRQHandler
    .thumb_set LCD_IRQHandler,Default_Handler

    .weak  CMP3_IRQHandler
    .thumb_set CMP3_IRQHandler,Default_Handler

    .weak  Tamper_IRQHandler
    .thumb_set Tamper_IRQHandler,Default_Handler

    .weak  Reserved116_IRQHandler
    .thumb_set Reserved116_IRQHandler,Default_Handler

    .weak  FTM3_IRQHandler
    .thumb_set ,Default_Handler

    .weak  ADC2_IRQHandler
    .thumb_set FTM3_IRQHandler,Default_Handler

    .weak  ADC3_IRQHandler
    .thumb_set ADC3_IRQHandler,Default_Handler

    .weak  I2S1_Tx_IRQHandler
    .thumb_set I2S1_Tx_IRQHandler,Default_Handler

    .weak  I2S1_Rx_IRQHandler
    .thumb_set I2S1_Rx_IRQHandler,Default_Handler


.end
