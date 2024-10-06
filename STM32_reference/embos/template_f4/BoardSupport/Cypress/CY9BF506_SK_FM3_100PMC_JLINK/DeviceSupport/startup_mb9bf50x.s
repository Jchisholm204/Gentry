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
File    : startup_mb9bf50x.s
Purpose : CMSIS startup code for Fujitsu MB9BF506
--------  END-OF-HEADER  ---------------------------------------------
*/


/* Vector Table Mapped to Address 0 at Reset */

    .section ".vectors","a",%progbits
    .globl  __Vectors
    .type   __Vectors, %object
__Vectors:
    .long   __stack_end__               /* Top of Stack                                */
    .long   Reset_Handler               /* Reset Handler                               */

    .long   NMI_Handler                 /* NMI                                         */
    .long   HardFault_Handler           /* Hard Fault                                  */
    .long   MemManage_Handler           /* MPU Fault                                   */
    .long   BusFault_Handler            /* Bus Fault                                   */
    .long   UsageFault_Handler          /* Usage Fault                                 */
    .long   0                           /* Reserved                                    */
    .long   0                           /* Reserved                                    */
    .long   0                           /* Reserved                                    */
    .long   0                           /* Reserved                                    */
    .long   SVC_Handler                 /* SVCall                                      */
    .long   DebugMon_Handler            /* Debug Monitor                               */
    .long   0                           /* Reserved                                    */
    .long   PendSV_Handler              /* PendSV                                      */
    .long   SysTick_Handler             /* SysTick                                     */

    .long   CSV_Handler                 /* 0: Clock Super Visor                        */
    .long   SWDT_Handler                /* 1: Software Watchdog Timer                  */
    .long   LVD_Handler                 /* 2: Low Voltage Detector                     */
    .long   MFT_WG_IRQHandler           /* 3: Wave Form Generator / DTIF               */
    .long   INT0_7_Handler              /* 4: External Interrupt Request ch.0 to ch.7  */
    .long   INT8_15_Handler             /* 5: External Interrupt Request ch.8 to ch.15 */
    .long   DT_Handler                  /* 6: Dual Timer / Quad Decoder                */
    .long   MFS0RX_IRQHandler           /* 7: MultiFunction Serial ch.0                */
    .long   MFS0TX_IRQHandler           /* 8: MultiFunction Serial ch.0                */
    .long   MFS1RX_IRQHandler           /* 9: MultiFunction Serial ch.1                */
    .long   MFS1TX_IRQHandler           /* 10: MultiFunction Serial ch.1               */
    .long   MFS2RX_IRQHandler           /* 11: MultiFunction Serial ch.2               */
    .long   MFS2TX_IRQHandler           /* 12: MultiFunction Serial ch.2               */
    .long   MFS3RX_IRQHandler           /* 13: MultiFunction Serial ch.3               */
    .long   MFS3TX_IRQHandler           /* 14: MultiFunction Serial ch.3               */
    .long   MFS4RX_IRQHandler           /* 15: MultiFunction Serial ch.4               */
    .long   MFS4TX_IRQHandler           /* 16: MultiFunction Serial ch.4               */
    .long   MFS5RX_IRQHandler           /* 17: MultiFunction Serial ch.5               */
    .long   MFS5TX_IRQHandler           /* 18: MultiFunction Serial ch.5               */
    .long   MFS6RX_IRQHandler           /* 19: MultiFunction Serial ch.6               */
    .long   MFS6TX_IRQHandler           /* 20: MultiFunction Serial ch.6               */
    .long   MFS7RX_IRQHandler           /* 21: MultiFunction Serial ch.7               */
    .long   MFS7TX_IRQHandler           /* 22: MultiFunction Serial ch.7               */
    .long   PPG_Handler                 /* 23: PPG                                     */
    .long   TIM_IRQHandler              /* 24: OSC / PLL / Watch Counter               */
    .long   ADC0_IRQHandler             /* 25: ADC0                                    */
    .long   ADC1_IRQHandler             /* 26: ADC1                                    */
    .long   ADC2_IRQHandler             /* 27: ADC2                                    */
    .long   MFT_FRT_IRQHandler          /* 28: Free-run Timer                          */
    .long   MFT_IPC_IRQHandler          /* 29: Input Capture                           */
    .long   MFT_OPC_IRQHandler          /* 30: Output Compare                          */
    .long   BT_IRQHandler               /* 31: Base Timer ch.0 to ch.7                 */
    .long   CAN0_IRQHandler             /* 32: CAN ch.0                                */
    .long   CAN1_IRQHandler             /* 33: CAN ch.1                                */
    .long   USBF_Handler                /* 34: USB Function                            */
    .long   USB_Handler                 /* 35: USB Function / USB HOST                 */
    .long   DummyHandler                /* 36: Reserved                                */
    .long   DummyHandler                /* 37: Reserved                                */
    .long   DMAC0_Handler               /* 38: DMAC ch.0                               */
    .long   DMAC1_Handler               /* 39: DMAC ch.1                               */
    .long   DMAC2_Handler               /* 40: DMAC ch.2                               */
    .long   DMAC3_Handler               /* 41: DMAC ch.3                               */
    .long   DMAC4_Handler               /* 42: DMAC ch.4                               */
    .long   DMAC5_Handler               /* 43: DMAC ch.5                               */
    .long   DMAC6_Handler               /* 44: DMAC ch.6                               */
    .long   DMAC7_Handler               /* 45: DMAC ch.7                               */
    .long   DummyHandler                /* 46: Reserved                                */
    .long   DummyHandler                /* 47: Reserved                                */

    .section ".text"
    .thumb

/* Reset Handler */
    .global Reset_Handler
    .cpu cortex-m3
    .thumb_func
    .syntax unified

Reset_Handler:
    .fnstart
    LDR     R0, =SystemInit
    BLX     R0

/* Copy the data segment initializers from flash to SRAM */
  movs	r1, #0
  b	LoopCopyDataInit

CopyDataInit:
	ldr	r3, =_sidata
	ldr	r3, [r3, r1]
	str	r3, [r0, r1]
	adds	r1, r1, #4

LoopCopyDataInit:
	ldr	r0, =_sdata
	ldr	r3, =_edata
	adds	r2, r0, r1
	cmp	r2, r3
	bcc	CopyDataInit
	ldr	r2, =_sbss
	b	LoopFillZerobss

/* Zero fill the bss segment. */
FillZerobss:
	movs	r3, #0
	str	r3, [r2], #4

LoopFillZerobss:
	ldr	r3, = _ebss
	cmp	r2, r3
	bcc	FillZerobss

/* Call static constructors */
    /*bl __libc_init_array*/
/* Call the application's entry point.*/
	bl	main
	bx	lr


/* Exception Handlers */

    .weak   NMI_Handler
    .type   NMI_Handler, %function
NMI_Handler:
    B       .
    .size   NMI_Handler, . - NMI_Handler

    .weak   HardFault_Handler
    .type   HardFault_Handler, %function
HardFault_Handler:
    B       .
    .size   HardFault_Handler, . - HardFault_Handler

    .weak   MemManage_Handler
    .type   MemManage_Handler, %function
MemManage_Handler:
    B       .
    .size   MemManage_Handler, . - MemManage_Handler

    .weak   BusFault_Handler
    .type   BusFault_Handler, %function
BusFault_Handler:
    B       .
    .size   BusFault_Handler, . - BusFault_Handler

    .weak   UsageFault_Handler
    .type   UsageFault_Handler, %function
UsageFault_Handler:
    B       .
    .size   UsageFault_Handler, . - UsageFault_Handler

    .weak   SVC_Handler
    .type   SVC_Handler, %function
SVC_Handler:
    B       .
    .size   SVC_Handler, . - SVC_Handler

    .weak   DebugMon_Handler
    .type   DebugMon_Handler, %function
DebugMon_Handler:
    B       .
    .size   DebugMon_Handler, . - DebugMon_Handler

    .weak   PendSV_Handler
    .type   PendSV_Handler, %function
PendSV_Handler:
    B       .
    .size   PendSV_Handler, . - PendSV_Handler

    .weak   SysTick_Handler
    .type   SysTick_Handler, %function
SysTick_Handler:
    B       .
    .size   SysTick_Handler, . - SysTick_Handler


    .global CSV_Handler
    .global SWDT_Handler
    .global LVD_Handler
    .global MFT_WG_IRQHandler
    .global INT0_7_Handler
    .global INT8_15_Handler
    .global DT_Handler
    .global MFS0RX_IRQHandler
    .global MFS0TX_IRQHandler
    .global MFS1RX_IRQHandler
    .global MFS1TX_IRQHandler
    .global MFS2RX_IRQHandler
    .global MFS2TX_IRQHandler
    .global MFS3RX_IRQHandler
    .global MFS3TX_IRQHandler
    .global MFS4RX_IRQHandler
    .global MFS4TX_IRQHandler
    .global MFS5RX_IRQHandler
    .global MFS5TX_IRQHandler
    .global MFS6RX_IRQHandler
    .global MFS6TX_IRQHandler
    .global MFS7RX_IRQHandler
    .global MFS7TX_IRQHandler
    .global PPG_Handler
    .global TIM_IRQHandler
    .global ADC0_IRQHandler
    .global ADC1_IRQHandler
    .global ADC2_IRQHandler
    .global MFT_FRT_IRQHandler
    .global MFT_IPC_IRQHandler
    .global MFT_OPC_IRQHandler
    .global BT_IRQHandler
    .global CAN0_IRQHandler
    .global CAN1_IRQHandler
    .global USBF_Handler
    .global USB_Handler
    .global DMAC0_Handler
    .global DMAC1_Handler
    .global DMAC2_Handler
    .global DMAC3_Handler
    .global DMAC4_Handler
    .global DMAC5_Handler
    .global DMAC6_Handler
    .global DMAC7_Handler
    .global DummyHandler


CSV_Handler:
SWDT_Handler:
LVD_Handler:
MFT_WG_IRQHandler:
INT0_7_Handler:
INT8_15_Handler:
DT_Handler:
MFS0RX_IRQHandler:
MFS0TX_IRQHandler:
MFS1RX_IRQHandler:
MFS1TX_IRQHandler:
MFS2RX_IRQHandler:
MFS2TX_IRQHandler:
MFS3RX_IRQHandler:
MFS3TX_IRQHandler:
MFS4RX_IRQHandler:
MFS4TX_IRQHandler:
MFS5RX_IRQHandler:
MFS5TX_IRQHandler:
MFS6RX_IRQHandler:
MFS6TX_IRQHandler:
MFS7RX_IRQHandler:
MFS7TX_IRQHandler:
PPG_Handler:
TIM_IRQHandler:
ADC0_IRQHandler:
ADC1_IRQHandler:
ADC2_IRQHandler:
MFT_FRT_IRQHandler:
MFT_IPC_IRQHandler:
MFT_OPC_IRQHandler:
BT_IRQHandler:
CAN0_IRQHandler:
CAN1_IRQHandler:
USBF_Handler:
USB_Handler:
DMAC0_Handler:
DMAC1_Handler:
DMAC2_Handler:
DMAC3_Handler:
DMAC4_Handler:
DMAC5_Handler:
DMAC6_Handler:
DMAC7_Handler:
DummyHandler:

        B       .



        .end
