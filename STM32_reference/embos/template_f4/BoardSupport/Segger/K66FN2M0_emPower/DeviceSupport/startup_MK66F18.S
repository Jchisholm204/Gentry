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

-------------------------- END-OF-HEADER -----------------------------
File    : startup_MK66F18.s
Purpose : CMSIS Startup code and vector table for Freescale K70
*/

    .syntax unified

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
**  Flash Config
**===========================================================================
*/
    .section    .flash_config,"a",%progbits

Backdoor:
  .byte 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
FPROT:
  .byte 0xff, 0xff, 0xff, 0xff
FSEC:
  .byte 0xfe
FOPT:
  .byte 0xff
FEPROT:
  .byte 0xff
FDPROT:
  .byte 0xff

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
    .long   __stack_end__                                   /* Top of Stack */
    .long   Reset_Handler                                   /* Reset Handler */
    .long   NMI_Handler                                     /* NMI Handler*/
    .long   HardFault_Handler                               /* Hard Fault Handler*/
    .long   MemManage_Handler                               /* MPU Fault Handler*/
    .long   BusFault_Handler                                /* Bus Fault Handler*/
    .long   UsageFault_Handler                              /* Usage Fault Handler*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   SVC_Handler                                     /* SVCall Handler*/
    .long   DebugMon_Handler                                /* Debug Monitor Handler*/
    .long   0                                               /* Reserved*/
    .long   PendSV_Handler                                  /* PendSV Handler*/
    .long   SysTick_Handler                                 /* SysTick Handler*/

                                                            /* External Interrupts*/
    .long   DMA0_DMA16_IRQHandler                           /* DMA Channel 0, 16 Transfer Complete*/
    .long   DMA1_DMA17_IRQHandler                           /* DMA Channel 1, 17 Transfer Complete*/
    .long   DMA2_DMA18_IRQHandler                           /* DMA Channel 2, 18 Transfer Complete*/
    .long   DMA3_DMA19_IRQHandler                           /* DMA Channel 3, 19 Transfer Complete*/
    .long   DMA4_DMA20_IRQHandler                           /* DMA Channel 4, 20 Transfer Complete*/
    .long   DMA5_DMA21_IRQHandler                           /* DMA Channel 5, 21 Transfer Complete*/
    .long   DMA6_DMA22_IRQHandler                           /* DMA Channel 6, 22 Transfer Complete*/
    .long   DMA7_DMA23_IRQHandler                           /* DMA Channel 7, 23 Transfer Complete*/
    .long   DMA8_DMA24_IRQHandler                           /* DMA Channel 8, 24 Transfer Complete*/
    .long   DMA9_DMA25_IRQHandler                           /* DMA Channel 9, 25 Transfer Complete*/
    .long   DMA10_DMA26_IRQHandler                          /* DMA Channel 10, 26 Transfer Complete*/
    .long   DMA11_DMA27_IRQHandler                          /* DMA Channel 11, 27 Transfer Complete*/
    .long   DMA12_DMA28_IRQHandler                          /* DMA Channel 12, 28 Transfer Complete*/
    .long   DMA13_DMA29_IRQHandler                          /* DMA Channel 13, 29 Transfer Complete*/
    .long   DMA14_DMA30_IRQHandler                          /* DMA Channel 14, 30 Transfer Complete*/
    .long   DMA15_DMA31_IRQHandler                          /* DMA Channel 15, 31 Transfer Complete*/
    .long   DMA_Error_IRQHandler                            /* DMA Error Interrupt*/
    .long   MCM_IRQHandler                                  /* Normal Interrupt*/
    .long   FTFE_IRQHandler                                 /* FTFE Command complete interrupt*/
    .long   Read_Collision_IRQHandler                       /* Read Collision Interrupt*/
    .long   LVD_LVW_IRQHandler                              /* Low Voltage Detect, Low Voltage Warning*/
    .long   LLWU_IRQHandler                                 /* Low Leakage Wakeup Unit*/
    .long   WDOG_EWM_IRQHandler                             /* WDOG Interrupt*/
    .long   RNG_IRQHandler                                  /* RNG Interrupt*/
    .long   I2C0_IRQHandler                                 /* I2C0 interrupt*/
    .long   I2C1_IRQHandler                                 /* I2C1 interrupt*/
    .long   SPI0_IRQHandler                                 /* SPI0 Interrupt*/
    .long   SPI1_IRQHandler                                 /* SPI1 Interrupt*/
    .long   I2S0_Tx_IRQHandler                              /* I2S0 transmit interrupt*/
    .long   I2S0_Rx_IRQHandler                              /* I2S0 receive interrupt*/
    .long   Reserved46_IRQHandler                           /* Reserved interrupt 46*/
    .long   UART0_RX_TX_IRQHandler                          /* UART0 Receive/Transmit interrupt*/
    .long   UART0_ERR_IRQHandler                            /* UART0 Error interrupt*/
    .long   UART1_RX_TX_IRQHandler                          /* UART1 Receive/Transmit interrupt*/
    .long   UART1_ERR_IRQHandler                            /* UART1 Error interrupt*/
    .long   UART2_RX_TX_IRQHandler                          /* UART2 Receive/Transmit interrupt*/
    .long   UART2_ERR_IRQHandler                            /* UART2 Error interrupt*/
    .long   UART3_RX_TX_IRQHandler                          /* UART3 Receive/Transmit interrupt*/
    .long   UART3_ERR_IRQHandler                            /* UART3 Error interrupt*/
    .long   ADC0_IRQHandler                                 /* ADC0 interrupt*/
    .long   CMP0_IRQHandler                                 /* CMP0 interrupt*/
    .long   CMP1_IRQHandler                                 /* CMP1 interrupt*/
    .long   FTM0_IRQHandler                                 /* FTM0 fault, overflow and channels interrupt*/
    .long   FTM1_IRQHandler                                 /* FTM1 fault, overflow and channels interrupt*/
    .long   FTM2_IRQHandler                                 /* FTM2 fault, overflow and channels interrupt*/
    .long   CMT_IRQHandler                                  /* CMT interrupt*/
    .long   RTC_IRQHandler                                  /* RTC interrupt*/
    .long   RTC_Seconds_IRQHandler                          /* RTC seconds interrupt*/
    .long   PIT0_IRQHandler                                 /* PIT timer channel 0 interrupt*/
    .long   PIT1_IRQHandler                                 /* PIT timer channel 1 interrupt*/
    .long   PIT2_IRQHandler                                 /* PIT timer channel 2 interrupt*/
    .long   PIT3_IRQHandler                                 /* PIT timer channel 3 interrupt*/
    .long   PDB0_IRQHandler                                 /* PDB0 Interrupt*/
    .long   USB0_IRQHandler                                 /* USB0 interrupt*/
    .long   USBDCD_IRQHandler                               /* USBDCD Interrupt*/
    .long   Reserved71_IRQHandler                           /* Reserved interrupt 71*/
    .long   DAC0_IRQHandler                                 /* DAC0 interrupt*/
    .long   MCG_IRQHandler                                  /* MCG Interrupt*/
    .long   LPTMR0_IRQHandler                               /* LPTimer interrupt*/
    .long   PORTA_IRQHandler                                /* Port A interrupt*/
    .long   PORTB_IRQHandler                                /* Port B interrupt*/
    .long   PORTC_IRQHandler                                /* Port C interrupt*/
    .long   PORTD_IRQHandler                                /* Port D interrupt*/
    .long   PORTE_IRQHandler                                /* Port E interrupt*/
    .long   SWI_IRQHandler                                  /* Software interrupt*/
    .long   SPI2_IRQHandler                                 /* SPI2 Interrupt*/
    .long   UART4_RX_TX_IRQHandler                          /* UART4 Receive/Transmit interrupt*/
    .long   UART4_ERR_IRQHandler                            /* UART4 Error interrupt*/
    .long   Reserved84_IRQHandler                           /* Reserved interrupt 84*/
    .long   Reserved85_IRQHandler                           /* Reserved interrupt 85*/
    .long   CMP2_IRQHandler                                 /* CMP2 interrupt*/
    .long   FTM3_IRQHandler                                 /* FTM3 fault, overflow and channels interrupt*/
    .long   DAC1_IRQHandler                                 /* DAC1 interrupt*/
    .long   ADC1_IRQHandler                                 /* ADC1 interrupt*/
    .long   I2C2_IRQHandler                                 /* I2C2 interrupt*/
    .long   CAN0_ORed_Message_buffer_IRQHandler             /* CAN0 OR'd message buffers interrupt*/
    .long   CAN0_Bus_Off_IRQHandler                         /* CAN0 bus off interrupt*/
    .long   CAN0_Error_IRQHandler                           /* CAN0 error interrupt*/
    .long   CAN0_Tx_Warning_IRQHandler                      /* CAN0 Tx warning interrupt*/
    .long   CAN0_Rx_Warning_IRQHandler                      /* CAN0 Rx warning interrupt*/
    .long   CAN0_Wake_Up_IRQHandler                         /* CAN0 wake up interrupt*/
    .long   SDHC_IRQHandler                                 /* SDHC interrupt*/
    .long   ENET_1588_Timer_IRQHandler                      /* Ethernet MAC IEEE 1588 Timer Interrupt*/
    .long   ENET_Transmit_IRQHandler                        /* Ethernet MAC Transmit Interrupt*/
    .long   ENET_Receive_IRQHandler                         /* Ethernet MAC Receive Interrupt*/
    .long   ENET_Error_IRQHandler                           /* Ethernet MAC Error and miscelaneous Interrupt*/
    .long   LPUART0_IRQHandler                              /* LPUART0 status/error interrupt*/
    .long   TSI0_IRQHandler                                 /* TSI0 interrupt*/
    .long   TPM1_IRQHandler                                 /* TPM1 fault, overflow and channels interrupt*/
    .long   TPM2_IRQHandler                                 /* TPM2 fault, overflow and channels interrupt*/
    .long   USBHSDCD_IRQHandler                             /* USBHSDCD, USBHS Phy Interrupt*/
    .long   I2C3_IRQHandler                                 /* I2C3 interrupt*/
    .long   CMP3_IRQHandler                                 /* CMP3 interrupt*/
    .long   USBHS_IRQHandler                                /* USB high speed OTG interrupt*/
    .long   CAN1_ORed_Message_buffer_IRQHandler             /* CAN1 OR'd message buffers interrupt*/
    .long   CAN1_Bus_Off_IRQHandler                         /* CAN1 bus off interrupt*/
    .long   CAN1_Error_IRQHandler                           /* CAN1 error interrupt*/
    .long   CAN1_Tx_Warning_IRQHandler                      /* CAN1 Tx warning interrupt*/
    .long   CAN1_Rx_Warning_IRQHandler                      /* CAN1 Rx warning interrupt*/
    .long   CAN1_Wake_Up_IRQHandler                         /* CAN1 wake up interrupt*/
    .long   DefaultISR                                      /* 116*/
    .long   DefaultISR                                      /* 117*/
    .long   DefaultISR                                      /* 118*/
    .long   DefaultISR                                      /* 119*/
    .long   DefaultISR                                      /* 120*/
    .long   DefaultISR                                      /* 121*/
    .long   DefaultISR                                      /* 122*/
    .long   DefaultISR                                      /* 123*/
    .long   DefaultISR                                      /* 124*/
    .long   DefaultISR                                      /* 125*/
    .long   DefaultISR                                      /* 126*/
    .long   DefaultISR                                      /* 127*/
    .long   DefaultISR                                      /* 128*/
    .long   DefaultISR                                      /* 129*/
    .long   DefaultISR                                      /* 130*/
    .long   DefaultISR                                      /* 131*/
    .long   DefaultISR                                      /* 132*/
    .long   DefaultISR                                      /* 133*/
    .long   DefaultISR                                      /* 134*/
    .long   DefaultISR                                      /* 135*/
    .long   DefaultISR                                      /* 136*/
    .long   DefaultISR                                      /* 137*/
    .long   DefaultISR                                      /* 138*/
    .long   DefaultISR                                      /* 139*/
    .long   DefaultISR                                      /* 140*/
    .long   DefaultISR                                      /* 141*/
    .long   DefaultISR                                      /* 142*/
    .long   DefaultISR                                      /* 143*/
    .long   DefaultISR                                      /* 144*/
    .long   DefaultISR                                      /* 145*/
    .long   DefaultISR                                      /* 146*/
    .long   DefaultISR                                      /* 147*/
    .long   DefaultISR                                      /* 148*/
    .long   DefaultISR                                      /* 149*/
    .long   DefaultISR                                      /* 150*/
    .long   DefaultISR                                      /* 151*/
    .long   DefaultISR                                      /* 152*/
    .long   DefaultISR                                      /* 153*/
    .long   DefaultISR                                      /* 154*/
    .long   DefaultISR                                      /* 155*/
    .long   DefaultISR                                      /* 156*/
    .long   DefaultISR                                      /* 157*/
    .long   DefaultISR                                      /* 158*/
    .long   DefaultISR                                      /* 159*/
    .long   DefaultISR                                      /* 160*/
    .long   DefaultISR                                      /* 161*/
    .long   DefaultISR                                      /* 162*/
    .long   DefaultISR                                      /* 163*/
    .long   DefaultISR                                      /* 164*/
    .long   DefaultISR                                      /* 165*/
    .long   DefaultISR                                      /* 166*/
    .long   DefaultISR                                      /* 167*/
    .long   DefaultISR                                      /* 168*/
    .long   DefaultISR                                      /* 169*/
    .long   DefaultISR                                      /* 170*/
    .long   DefaultISR                                      /* 171*/
    .long   DefaultISR                                      /* 172*/
    .long   DefaultISR                                      /* 173*/
    .long   DefaultISR                                      /* 174*/
    .long   DefaultISR                                      /* 175*/
    .long   DefaultISR                                      /* 176*/
    .long   DefaultISR                                      /* 177*/
    .long   DefaultISR                                      /* 178*/
    .long   DefaultISR                                      /* 179*/
    .long   DefaultISR                                      /* 180*/
    .long   DefaultISR                                      /* 181*/
    .long   DefaultISR                                      /* 182*/
    .long   DefaultISR                                      /* 183*/
    .long   DefaultISR                                      /* 184*/
    .long   DefaultISR                                      /* 185*/
    .long   DefaultISR                                      /* 186*/
    .long   DefaultISR                                      /* 187*/
    .long   DefaultISR                                      /* 188*/
    .long   DefaultISR                                      /* 189*/
    .long   DefaultISR                                      /* 190*/
    .long   DefaultISR                                      /* 191*/
    .long   DefaultISR                                      /* 192*/
    .long   DefaultISR                                      /* 193*/
    .long   DefaultISR                                      /* 194*/
    .long   DefaultISR                                      /* 195*/
    .long   DefaultISR                                      /* 196*/
    .long   DefaultISR                                      /* 197*/
    .long   DefaultISR                                      /* 198*/
    .long   DefaultISR                                      /* 199*/
    .long   DefaultISR                                      /* 200*/
    .long   DefaultISR                                      /* 201*/
    .long   DefaultISR                                      /* 202*/
    .long   DefaultISR                                      /* 203*/
    .long   DefaultISR                                      /* 204*/
    .long   DefaultISR                                      /* 205*/
    .long   DefaultISR                                      /* 206*/
    .long   DefaultISR                                      /* 207*/
    .long   DefaultISR                                      /* 208*/
    .long   DefaultISR                                      /* 209*/
    .long   DefaultISR                                      /* 210*/
    .long   DefaultISR                                      /* 211*/
    .long   DefaultISR                                      /* 212*/
    .long   DefaultISR                                      /* 213*/
    .long   DefaultISR                                      /* 214*/
    .long   DefaultISR                                      /* 215*/
    .long   DefaultISR                                      /* 216*/
    .long   DefaultISR                                      /* 217*/
    .long   DefaultISR                                      /* 218*/
    .long   DefaultISR                                      /* 219*/
    .long   DefaultISR                                      /* 220*/
    .long   DefaultISR                                      /* 221*/
    .long   DefaultISR                                      /* 222*/
    .long   DefaultISR                                      /* 223*/
    .long   DefaultISR                                      /* 224*/
    .long   DefaultISR                                      /* 225*/
    .long   DefaultISR                                      /* 226*/
    .long   DefaultISR                                      /* 227*/
    .long   DefaultISR                                      /* 228*/
    .long   DefaultISR                                      /* 229*/
    .long   DefaultISR                                      /* 230*/
    .long   DefaultISR                                      /* 231*/
    .long   DefaultISR                                      /* 232*/
    .long   DefaultISR                                      /* 233*/
    .long   DefaultISR                                      /* 234*/
    .long   DefaultISR                                      /* 235*/
    .long   DefaultISR                                      /* 236*/
    .long   DefaultISR                                      /* 237*/
    .long   DefaultISR                                      /* 238*/
    .long   DefaultISR                                      /* 239*/
    .long   DefaultISR                                      /* 240*/
    .long   DefaultISR                                      /* 241*/
    .long   DefaultISR                                      /* 242*/
    .long   DefaultISR                                      /* 243*/
    .long   DefaultISR                                      /* 244*/
    .long   DefaultISR                                      /* 245*/
    .long   DefaultISR                                      /* 246*/
    .long   DefaultISR                                      /* 247*/
    .long   DefaultISR                                      /* 248*/
    .long   DefaultISR                                      /* 249*/
    .long   DefaultISR                                      /* 250*/
    .long   DefaultISR                                      /* 251*/
    .long   DefaultISR                                      /* 252*/
    .long   DefaultISR                                      /* 253*/
    .long   DefaultISR                                      /* 254*/
    .long   0xFFFFFFFF                                      /*  Reserved for user TRIM value*/

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

    .weak  LLWU_IRQHandler
    .thumb_set LLW_IRQHandler,Default_Handler

    .weak  WDOG_EWM_IRQHandler
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

    .weak  I2S0_Tx_IRQHandler
    .thumb_set I2S0_Tx_IRQHandler,Default_Handler

    .weak  I2S0_Rx_IRQHandler
    .thumb_set I2S0_Rx_IRQHandler,Default_Handler

    .weak  Reserved46_IRQHandler
    .thumb_set Reserved46_IRQHandler,Default_Handler

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

    .weak  ADC0_IRQHandler
    .thumb_set ADC0_IRQHandler,Default_Handler

    .weak  CMP0_IRQHandler
    .thumb_set CMP0_IRQHandler,Default_Handler

    .weak  CMP1_IRQHandler
    .thumb_set CMP1_IRQHandler,Default_Handler

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

    .weak  Reserved71_IRQHandler
    .thumb_set Reserved71_IRQHandler,Default_Handler

    .weak  DAC0_IRQHandler
    .thumb_set DAC0_IRQHandler,Default_Handler

    .weak  MCG_IRQHandler
    .thumb_set MCG_IRQHandler,Default_Handler

    .weak  LPTMR0_IRQHandler
    .thumb_set LPTMR0_IRQHandler,Default_Handler

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

    .weak  SWI_IRQHandler
    .thumb_set SWI_IRQHandler,Default_Handler

    .weak  SPI2_IRQHandler
    .thumb_set SPI2_IRQHandler,Default_Handler

    .weak  UART4_RX_TX_IRQHandler
    .thumb_set UART4_RX_TX_IRQHandler,Default_Handler

    .weak  UART4_ERR_IRQHandler
    .thumb_set UART4_ERR_IRQHandler,Default_Handler

    .weak  Reserved84_IRQHandler
    .thumb_set Reserved84_IRQHandler,Default_Handler

    .weak  Reserved85_IRQHandler
    .thumb_set Reserved85_IRQHandler,Default_Handler

    .weak  CMP2_IRQHandler
    .thumb_set CMP2_IRQHandler,Default_Handler

    .weak  FTM3_IRQHandler
    .thumb_set FTM3_IRQHandler,Default_Handler

    .weak  DAC1_IRQHandler
    .thumb_set DAC1_IRQHandler,Default_Handler

    .weak  ADC1_IRQHandler
    .thumb_set ADC1_IRQHandler,Default_Handler

    .weak  I2C2_IRQHandler
    .thumb_set I2C2_IRQHandler,Default_Handler

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

    .weak  SDHC_IRQHandler
    .thumb_set SDHC_IRQHandler,Default_Handler

    .weak  ENET_1588_Timer_IRQHandler
    .thumb_set ENET_1588_Timer_IRQHandler,Default_Handler

    .weak  ENET_Transmit_IRQHandler
    .thumb_set ENET_Transmit_IRQHandler,Default_Handler

    .weak  ENET_Receive_IRQHandler
    .thumb_set ENET_Receive_IRQHandler,Default_Handler

    .weak  ENET_Error_IRQHandler
    .thumb_set ENET_Error_IRQHandler,Default_Handler

    .weak  LPUART0_IRQHandler
    .thumb_set LPUART0_IRQHandler,Default_Handler

    .weak  TSI0_IRQHandler
    .thumb_set TSI0_IRQHandler,Default_Handler

    .weak  TPM1_IRQHandler
    .thumb_set TPM1_IRQHandler,Default_Handler

    .weak  TPM2_IRQHandler
    .thumb_set TPM2_IRQHandler,Default_Handler

    .weak  USBHSDCD_IRQHandler
    .thumb_set USBHSDCD_IRQHandler,Default_Handler

    .weak  I2C3_IRQHandler
    .thumb_set I2C3_IRQHandler,Default_Handler

    .weak  CMP3_IRQHandler
    .thumb_set CMP3_IRQHandler,Default_Handler

    .weak  USBHS_IRQHandler
    .thumb_set USBHS_IRQHandler,Default_Handler

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

    .weak  DefaultISR
    .thumb_set DefaultISR,Default_Handler

    .end
