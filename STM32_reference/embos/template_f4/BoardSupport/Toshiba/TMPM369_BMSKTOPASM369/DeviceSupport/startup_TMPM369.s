;/**
; ****************************************************************************
; * @file     startup_TMPM369.s
; * @brief    CMSIS Cortex-M3 Core Device Startup File for the
; *           TOSHIBA 'TMPM369' Device Series 
; * @version  V2.0.2.2 (Tentative)
; * @date     2010/09/29
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; * 
; * THE SOURCE CODE AND ITS RELATED DOCUMENTATION IS PROVIDED "AS IS". TOSHIBA
; * CORPORATION MAKES NO OTHER WARRANTY OF ANY KIND, WHETHER EXPRESS, IMPLIED OR,
; * STATUTORY AND DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF MERCHANTABILITY,
; * SATISFACTORY QUALITY, NON INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
; * 
; * THE SOURCE CODE AND DOCUMENTATION MAY INCLUDE ERRORS. TOSHIBA CORPORATION
; * RESERVES THE RIGHT TO INCORPORATE MODIFICATIONS TO THE SOURCE CODE IN LATER
; * REVISIONS OF IT, AND TO MAKE IMPROVEMENTS OR CHANGES IN THE DOCUMENTATION OR
; * THE PRODUCTS OR TECHNOLOGIES DESCRIBED THEREIN AT ANY TIME.
; * 
; * TOSHIBA CORPORATION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
; * CONSEQUENTIAL DAMAGE OR LIABILITY ARISING FROM YOUR USE OF THE SOURCE CODE OR
; * ANY DOCUMENTATION, INCLUDING BUT NOT LIMITED TO, LOST REVENUES, DATA OR
; * PROFITS, DAMAGES OF ANY SPECIAL, INCIDENTAL OR CONSEQUENTIAL NATURE, PUNITIVE
; * DAMAGES, LOSS OF PROPERTY OR LOSS OF PROFITS ARISING OUT OF OR IN CONNECTION
; * WITH THIS AGREEMENT, OR BEING UNUSABLE, EVEN IF ADVISED OF THE POSSIBILITY OR
; * PROBABILITY OF SUCH DAMAGES AND WHETHER A CLAIM FOR SUCH DAMAGE IS BASED UPON
; * WARRANTY, CONTRACT, TORT, NEGLIGENCE OR OTHERWISE.
; * 
; * (C)Copyright TOSHIBA CORPORATION 2010 All rights reserved
; *****************************************************************************
; */




/* Vector Table Mapped to Address 0 at Reset */

    .section ".vectors","a",%progbits
    .globl  __Vectors
    .type   __Vectors, %object

__Vectors:
    .long   __stack_end__               /* Top of Stack                 */
    .long   Reset_Handler               /* Reset Handler                */
    .long   NMI_Handler                 /* NMI Handler                  */
    .long   HardFault_Handler           /* Hard Fault Handler           */
    .long   MemManage_Handler           /* MPU Fault Handler            */
    .long   BusFault_Handler            /* Bus Fault Handler            */
    .long   UsageFault_Handler          /* Usage Fault Handler          */
    .long   0                           /* Reserved                     */
    .long   0                           /* Reserved                     */
    .long   0                           /* Reserved                     */
    .long   0                           /* Reserved                     */
    .long   SVC_Handler                 /* SVCall Handler               */
    .long   DebugMon_Handler            /* Debug Monitor Handler        */
    .long   0                           /* Reserved                     */
    .long   PendSV_Handler              /* PendSV Handler               */
    .long   SysTick_Handler             /* SysTick Handler              */

    /* External Interrupts */
    .long   INT0_IRQHandler           /* 0:   Interrupt pin 0                                                        */                                             
    .long   INT1_IRQHandler           /* 1:   Interrupt pin 1                                                        */
    .long   INT2_IRQHandler           /* 2:   Interrupt pin 2                                                        */
    .long   INT3_IRQHandler           /* 3:   Interrupt pin 3                                                        */
    .long   INT4_IRQHandler           /* 4:   Interrupt pin 4                                                        */
    .long   INT5_IRQHandler           /* 5:   Interrupt pin 5                                                        */
    .long   INT6_IRQHandler           /* 6:   Interrupt pin 6                                                        */
    .long   INT7_IRQHandler           /* 7:   Interrupt pin 7                                                        */
    .long   INT8_IRQHandler           /* 8:   Interrupt pin 8                                                        */
    .long   INT9_IRQHandler           /* 9:   Interrupt pin 9                                                        */
    .long   INTA_IRQHandler           /* 10:  Interrupt pin A                                                        */
    .long   INTB_IRQHandler           /* 11:  Interrupt pin B                                                        */
    .long   INTC_IRQHandler           /* 12:  Interrupt pin C                                                        */
    .long   INTD_IRQHandler           /* 13:  Interrupt pin D                                                        */
    .long   INTE_IRQHandler           /* 14:  Interrupt pin E                                                        */
    .long   INTF_IRQHandler           /* 15:  Interrupt pin F                                                        */
    .long   INTRX0_IRQHandler         /* 16:  Serial reception (channel.0)                                           */
    .long   INTTX0_IRQHandler         /* 17:  Serial transmission (channel.0)                                        */
    .long   INTRX1_IRQHandler         /* 18:  Serial reception (channel.1)                                           */
    .long   INTTX1_IRQHandler         /* 19:  Serial transmission (channel.1)                                        */
    .long   INTRX2_IRQHandler         /* 20:  Serial reception (channel.2)                                           */
    .long   INTTX2_IRQHandler         /* 21:  Serial transmission (channel.2)                                        */
    .long   INTRX3_IRQHandler         /* 22:  Serial reception (channel.3)                                           */
    .long   INTTX3_IRQHandler         /* 23:  Serial transmission (channel.3)                                        */
    .long   INTUART4_IRQHandler       /* 24:  FULL UART(channel.4)                                                   */
    .long   INTUART5_IRQHandler       /* 25:  FULL UART(channel.5)                                                   */
    .long   INTSBI0_IRQHandler        /* 26:  Serial bus interface 0                                                 */
    .long   INTSBI1_IRQHandler        /* 27:  Serial bus interface 1                                                 */
    .long   INTSBI2_IRQHandler        /* 28:  Serial bus interface 2                                                 */
    .long   INTSSP0_IRQHandler        /* 29:  SPI serial interface 0                                                 */
    .long   INTSSP1_IRQHandler        /* 30:  SPI serial interface 1                                                 */
    .long   INTSSP2_IRQHandler        /* 31:  SPI serial interface 2                                                 */
    .long   INTUSBH_IRQHandler        /* 32:  USB Host Interrupt                                                     */
    .long   INTUSBD_IRQHandler        /* 33:  USB Device Interrupt                                                   */
    .long   INTUSBWKUP_IRQHandler     /* 34:  USB WakeUp                                                             */
    .long   INTCANRX_IRQHandler       /* 35:  CAN RX                                                                 */
    .long   INTCANTX_IRQHandler       /* 36:  CAN TX                                                                 */
    .long   INTCANGB_IRQHandler       /* 37:  CAN STAUTS                                                             */
    .long   INTETH_IRQHandler         /* 38:  EtherNET Interrupt                                                     */
    .long   INTETHWK_IRQHandler       /* 39:  EtherNET(magic packet detection) interrupt                             */
    .long   INTADAHP_IRQHandler       /* 40:  Highest priority AD conversion complete interrupt (channel.A)          */
    .long   INTADAM0_IRQHandler       /* 41:  AD conversion monitoring function interrupt 0(channel.A)               */
    .long   INTADAM1_IRQHandler       /* 42:  AD conversion monitoring function interrupt 1(channel.A)               */
    .long   INTADA_IRQHandler         /* 43:  AD conversion interrupt(channel.A)                                     */
    .long   INTADBHP_IRQHandler       /* 44:  Highest priority AD conversion complete interrupt (channel.B)          */
    .long   INTADBM0_IRQHandler       /* 45:  AD conversion monitoring function interrupt 0(channel.B)               */
    .long   INTADBM1_IRQHandler       /* 46:  AD conversion monitoring function interrupt 1(channel.B)               */
    .long   INTADB_IRQHandler         /* 47:  AD conversion interrupt(channel.B)                                     */
    .long   INTEMG0_IRQHandler        /* 48:  PMD0 EMG interrupt (MPT0)                                              */
    .long   INTPMD0_IRQHandler        /* 49:  PMD0 PWM interrupt (MPT0)                                              */
    .long   INTENC0_IRQHandler        /* 50:  PMD0 Encoder input interrupt (MPT0)                                    */
    .long   INTEMG1_IRQHandler        /* 51:  PMD1 EMG interrupt (MPT1)                                              */
    .long   INTPMD1_IRQHandler        /* 52:  PMD1 PWM interrupt (MPT1)                                              */
    .long   INTENC1_IRQHandler        /* 53:  PMD1 Encoder input interrupt (MPT1)                                    */
    .long   INTMTEMG0_IRQHandler      /* 54:  16-bit MPT0 IGBT EMG interrupt                                         */
    .long   INTMTPTB00_IRQHandler     /* 55:  16-bit MPT0 IGBT period/ TMRB compare match detection 0                */
    .long   INTMTTTB01_IRQHandler     /* 56:  16-bit MPT0 IGBT trigger/ TMRB compare match detection 1               */
    .long   INTMTCAP00_IRQHandler     /* 57:  16-bit MPT0 input capture 0                                            */
    .long   INTMTCAP01_IRQHandler     /* 58:  16-bit MPT0 input capture 1                                            */
    .long   INTMTEMG1_IRQHandler      /* 59:  16-bit MPT1 IGBT EMG interrupt                                         */
    .long   INTMTPTB10_IRQHandler     /* 60:  16-bit MPT1 IGBT period/ TMRB compare match detection 0                */
    .long   INTMTTTB11_IRQHandler     /* 61:  16-bit MPT1 IGBT trigger/ TMRB compare match detection 1               */
    .long   INTMTCAP10_IRQHandler     /* 62:  16-bit MPT1 input capture 0                                            */
    .long   INTMTCAP11_IRQHandler     /* 63:  16-bit MPT1 input capture 1                                            */
    .long   INTMTEMG2_IRQHandler      /* 64:  16-bit MPT2 IGBT EMG interrupt                                         */
    .long   INTMTPTB20_IRQHandler     /* 65:  16-bit MPT2 IGBT period/ TMRB compare match detection 0                */
    .long   INTMTTTB21_IRQHandler     /* 66:  16-bit MPT2 IGBT trigger/ TMRB compare match detection 1               */
    .long   INTMTCAP20_IRQHandler     /* 67:  16-bit MPT2 input capture 0                                            */
    .long   INTMTCAP21_IRQHandler     /* 68:  16-bit MPT2 input capture 1                                            */
    .long   INTMTEMG3_IRQHandler      /* 69:  16-bit MPT3 IGBT EMG interrupt                                         */
    .long   INTMTPTB30_IRQHandler     /* 70:  16-bit MPT3 IGBT period/ TMRB compare match detection 0                */
    .long   INTMTTTB31_IRQHandler     /* 71:  16-bit MPT3 IGBT trigger/ TMRB compare match detection 1               */
    .long   INTMTCAP30_IRQHandler     /* 72:  16-bit MPT3 input capture 0                                            */
    .long   INTMTCAP31_IRQHandler     /* 73:  16-bit MPT3 input capture 1                                            */
    .long   INTRMCRX_IRQHandler       /* 74:  Remote Controller reception interrupt                                  */
    .long   INTTB0_IRQHandler         /* 75:  16-bit TMRB_0 match detection 0                                        */
    .long   INTCAP00_IRQHandler       /* 76:  16-bit TMRB_0 input capture 00                                         */
    .long   INTCAP01_IRQHandler       /* 77:  16-bit TMRB_0 input capture 01                                         */
    .long   INTTB1_IRQHandler         /* 78:  16-bit TMRB_1 match detection 1                                        */
    .long   INTCAP10_IRQHandler       /* 79:  16-bit TMRB_1 input capture 10                                         */
    .long   INTCAP11_IRQHandler       /* 80:  16-bit TMRB_1 input capture 11                                         */
    .long   INTTB2_IRQHandler         /* 81:  16-bit TMRB_2 match detection 2                                        */
    .long   INTCAP20_IRQHandler       /* 82:  16-bit TMRB_2 input capture 20                                         */
    .long   INTCAP21_IRQHandler       /* 83:  16-bit TMRB_2 input capture 21                                         */
    .long   INTTB3_IRQHandler         /* 84:  16-bit TMRB_3 match detection 3                                        */
    .long   INTCAP30_IRQHandler       /* 85:  16-bit TMRB_3 input capture 30                                         */
    .long   INTCAP31_IRQHandler       /* 86:  16-bit TMRB_3 input capture 31                                         */
    .long   INTTB4_IRQHandler         /* 87:  16-bit TMRB_4 match detection 4                                        */
    .long   INTCAP40_IRQHandler       /* 88:  16-bit TMRB_4 input capture 40                                         */
    .long   INTCAP41_IRQHandler       /* 89:  16-bit TMRB_4 input capture 41                                         */
    .long   INTTB5_IRQHandler         /* 90:  16-bit TMRB_5 match detection 5                                        */
    .long   INTCAP50_IRQHandler       /* 91:  16-bit TMRB_5 input capture 50                                         */
    .long   INTCAP51_IRQHandler       /* 92:  16-bit TMRB_5 input capture 51                                         */
    .long   INTTB6_IRQHandler         /* 93:  16-bit TMRB_6 match detection 6                                        */
    .long   INTCAP60_IRQHandler       /* 94:  16-bit TMRB_6 input capture 60                                         */
    .long   INTCAP61_IRQHandler       /* 95:  16-bit TMRB_6 input capture 61                                         */
    .long   INTTB7_IRQHandler         /* 96:  16-bit TMRB_7 match detection 7                                        */
    .long   INTCAP70_IRQHandler       /* 97:  16-bit TMRB_7 input capture 70                                         */
    .long   INTCAP71_IRQHandler       /* 98:  16-bit TMRB_7 input capture 71                                         */
    .long   INTRTC_IRQHandler         /* 99:  RTC(Real time clock) interrupt                                         */
    .long   INTDMAADA_IRQHandler      /* 100: DMA_ADC_A conversion end                                               */
    .long   INTDMAADB_IRQHandler      /* 101: DMA_ADC_B conversion end                                               */
    .long   INTDMADAA_IRQHandler      /* 102: DMA_DAC_A conversion trigger                                           */
    .long   INTDMADAB_IRQHandler      /* 103: DMA_DAC_B conversion trigger                                           */
    .long   INTDMASPR0_IRQHandler     /* 104: DMA_SSP_0 reception / DMA_I2C SIO_0                                    */
    .long   INTDMASPT0_IRQHandler     /* 105: DMA_SSP_0 transmission                                                 */
    .long   INTDMASPR1_IRQHandler     /* 106: DMA_SSP_1 reception                                                    */
    .long   INTDMASPT1_IRQHandler     /* 107: DMA_SSP_1 transmission                                                 */
    .long   INTDMASPR2_IRQHandler     /* 108: DMA_SSP_2 reception                                                    */
    .long   INTDMASPT2_IRQHandler     /* 109: DMA_SSP_2 transmission                                                 */
    .long   INTDMAUTR4_IRQHandler     /* 110: DMA_FUART_4 reception                                                  */
    .long   INTDMAUTT4_IRQHandler     /* 111: DMA_FUART_4 transmission                                               */
    .long   INTDMAUTR5_IRQHandler     /* 112: DMA_FUART_5 reception                                                  */
    .long   INTDMAUTT5_IRQHandler     /* 113: DMA_FUART_5 transmission                                               */
    .long   INTDMARX0_IRQHandler      /* 114: DMA_SIO/ UART_0 reception                                              */
    .long   INTDMATX0_IRQHandler      /* 115: DMA_SIO/ UART_0 transmission                                           */
    .long   INTDMARX1_IRQHandler      /* 116: DMA_SIO/ UART_1 reception                                              */
    .long   INTDMATX1_IRQHandler      /* 117: DMA_SIO/ UART_1 transmission                                           */
    .long   INTDMARX2_IRQHandler      /* 118: DMA_SIO/ UART_2 reception                                              */
    .long   INTDMATX2_IRQHandler      /* 119: DMA_SIO/ UART_2 transmission                                           */
    .long   INTDMARX3_IRQHandler      /* 120: DMA_SIO/ UART_3 reception                                              */
    .long   INTDMATX3_IRQHandler      /* 121: DMA_SIO/ UART_3 transmission                                           */
    .long   INTDMASBI1_IRQHandler     /* 122: DMA_I2C/ SIO_1                                                         */
    .long   INTDMASBI2_IRQHandler     /* 123: DMA_I2C/ SIO_2                                                         */
    .long   INTDMATB_IRQHandler       /* 124: 16-bit TMRB_0-4 match detection                                        */
    .long   INTDMARQ_IRQHandler       /* 125: DMA request pin                                                        */
    .long   INTDMAAERR_IRQHandler     /* 126: DMA_A error transfer interrupt                                         */
    .long   INTDMABERR_IRQHandler     /*; 127: DMA_B error transfer interrupt                                        */


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
    bl __libc_init_array
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


    .global  INT0_IRQHandler           
    .global  INT1_IRQHandler           
    .global  INT2_IRQHandler           
    .global  INT3_IRQHandler           
    .global  INT4_IRQHandler           
    .global  INT5_IRQHandler           
    .global  INT6_IRQHandler           
    .global  INT7_IRQHandler           
    .global  INT8_IRQHandler           
    .global  INT9_IRQHandler           
    .global  INTA_IRQHandler           
    .global  INTB_IRQHandler           
    .global  INTC_IRQHandler           
    .global  INTD_IRQHandler           
    .global  INTE_IRQHandler           
    .global  INTF_IRQHandler           
    .global  INTRX0_IRQHandler         
    .global  INTTX0_IRQHandler         
    .global  INTRX1_IRQHandler         
    .global  INTTX1_IRQHandler         
    .global  INTRX2_IRQHandler         
    .global  INTTX2_IRQHandler         
    .global  INTRX3_IRQHandler         
    .global  INTTX3_IRQHandler         
    .global  INTUART4_IRQHandler       
    .global  INTUART5_IRQHandler       
    .global  INTSBI0_IRQHandler        
    .global  INTSBI1_IRQHandler        
    .global  INTSBI2_IRQHandler        
    .global  INTSSP0_IRQHandler        
    .global  INTSSP1_IRQHandler        
    .global  INTSSP2_IRQHandler        
    .global  INTUSBH_IRQHandler        
    .global  INTUSBD_IRQHandler        
    .global  INTUSBWKUP_IRQHandler     
    .global  INTCANRX_IRQHandler       
    .global  INTCANTX_IRQHandler       
    .global  INTCANGB_IRQHandler       
    .global  INTETH_IRQHandler         
    .global  INTETHWK_IRQHandler       
    .global  INTADAHP_IRQHandler       
    .global  INTADAM0_IRQHandler       
    .global  INTADAM1_IRQHandler       
    .global  INTADA_IRQHandler         
    .global  INTADBHP_IRQHandler       
    .global  INTADBM0_IRQHandler       
    .global  INTADBM1_IRQHandler       
    .global  INTADB_IRQHandler         
    .global  INTEMG0_IRQHandler        
    .global  INTPMD0_IRQHandler        
    .global  INTENC0_IRQHandler        
    .global  INTEMG1_IRQHandler        
    .global  INTPMD1_IRQHandler        
    .global  INTENC1_IRQHandler        
    .global  INTMTEMG0_IRQHandler      
    .global  INTMTPTB00_IRQHandler     
    .global  INTMTTTB01_IRQHandler     
    .global  INTMTCAP00_IRQHandler     
    .global  INTMTCAP01_IRQHandler     
    .global  INTMTEMG1_IRQHandler      
    .global  INTMTPTB10_IRQHandler     
    .global  INTMTTTB11_IRQHandler     
    .global  INTMTCAP10_IRQHandler     
    .global  INTMTCAP11_IRQHandler     
    .global  INTMTEMG2_IRQHandler      
    .global  INTMTPTB20_IRQHandler     
    .global  INTMTTTB21_IRQHandler     
    .global  INTMTCAP20_IRQHandler     
    .global  INTMTCAP21_IRQHandler     
    .global  INTMTEMG3_IRQHandler      
    .global  INTMTPTB30_IRQHandler     
    .global  INTMTTTB31_IRQHandler     
    .global  INTMTCAP30_IRQHandler     
    .global  INTMTCAP31_IRQHandler     
    .global  INTRMCRX_IRQHandler       
    .global  INTTB0_IRQHandler         
    .global  INTCAP00_IRQHandler       
    .global  INTCAP01_IRQHandler       
    .global  INTTB1_IRQHandler         
    .global  INTCAP10_IRQHandler       
    .global  INTCAP11_IRQHandler       
    .global  INTTB2_IRQHandler         
    .global  INTCAP20_IRQHandler       
    .global  INTCAP21_IRQHandler       
    .global  INTTB3_IRQHandler         
    .global  INTCAP30_IRQHandler       
    .global  INTCAP31_IRQHandler       
    .global  INTTB4_IRQHandler         
    .global  INTCAP40_IRQHandler       
    .global  INTCAP41_IRQHandler       
    .global  INTTB5_IRQHandler         
    .global  INTCAP50_IRQHandler       
    .global  INTCAP51_IRQHandler       
    .global  INTTB6_IRQHandler         
    .global  INTCAP60_IRQHandler       
    .global  INTCAP61_IRQHandler       
    .global  INTTB7_IRQHandler         
    .global  INTCAP70_IRQHandler       
    .global  INTCAP71_IRQHandler       
    .global  INTRTC_IRQHandler         
    .global  INTDMAADA_IRQHandler      
    .global  INTDMAADB_IRQHandler      
    .global  INTDMADAA_IRQHandler      
    .global  INTDMADAB_IRQHandler      
    .global  INTDMASPR0_IRQHandler     
    .global  INTDMASPT0_IRQHandler     
    .global  INTDMASPR1_IRQHandler     
    .global  INTDMASPT1_IRQHandler     
    .global  INTDMASPR2_IRQHandler     
    .global  INTDMASPT2_IRQHandler     
    .global  INTDMAUTR4_IRQHandler     
    .global  INTDMAUTT4_IRQHandler     
    .global  INTDMAUTR5_IRQHandler     
    .global  INTDMAUTT5_IRQHandler     
    .global  INTDMARX0_IRQHandler      
    .global  INTDMATX0_IRQHandler      
    .global  INTDMARX1_IRQHandler      
    .global  INTDMATX1_IRQHandler      
    .global  INTDMARX2_IRQHandler      
    .global  INTDMATX2_IRQHandler      
    .global  INTDMARX3_IRQHandler      
    .global  INTDMATX3_IRQHandler      
    .global  INTDMASBI1_IRQHandler     
    .global  INTDMASBI2_IRQHandler     
    .global  INTDMATB_IRQHandler       
    .global  INTDMARQ_IRQHandler       
    .global  INTDMAAERR_IRQHandler     
    .global  INTDMABERR_IRQHandler     
                        
INT0_IRQHandler:
INT1_IRQHandler:
INT2_IRQHandler:
INT3_IRQHandler:
INT4_IRQHandler:
INT5_IRQHandler:
INT6_IRQHandler:
INT7_IRQHandler:
INT8_IRQHandler:
INT9_IRQHandler:
INTA_IRQHandler:
INTB_IRQHandler:
INTC_IRQHandler:
INTD_IRQHandler:
INTE_IRQHandler:
INTF_IRQHandler:
INTRX0_IRQHandler:
INTTX0_IRQHandler:
INTRX1_IRQHandler:
INTTX1_IRQHandler:
INTRX2_IRQHandler:
INTTX2_IRQHandler:
INTRX3_IRQHandler:
INTTX3_IRQHandler:
INTUART4_IRQHandler:
INTUART5_IRQHandler:
INTSBI0_IRQHandler:
INTSBI1_IRQHandler:
INTSBI2_IRQHandler:
INTSSP0_IRQHandler:
INTSSP1_IRQHandler:
INTSSP2_IRQHandler:
INTUSBH_IRQHandler:
INTUSBD_IRQHandler:
INTUSBWKUP_IRQHandler:
INTCANRX_IRQHandler:
INTCANTX_IRQHandler:
INTCANGB_IRQHandler:
INTETH_IRQHandler:
INTETHWK_IRQHandler:
INTADAHP_IRQHandler:
INTADAM0_IRQHandler:
INTADAM1_IRQHandler:
INTADA_IRQHandler:
INTADBHP_IRQHandler:
INTADBM0_IRQHandler:
INTADBM1_IRQHandler:
INTADB_IRQHandler:
INTEMG0_IRQHandler:
INTPMD0_IRQHandler:
INTENC0_IRQHandler:
INTEMG1_IRQHandler:
INTPMD1_IRQHandler:
INTENC1_IRQHandler:
INTMTEMG0_IRQHandler:
INTMTPTB00_IRQHandler:
INTMTTTB01_IRQHandler:
INTMTCAP00_IRQHandler:
INTMTCAP01_IRQHandler:
INTMTEMG1_IRQHandler:
INTMTPTB10_IRQHandler:
INTMTTTB11_IRQHandler:
INTMTCAP10_IRQHandler:
INTMTCAP11_IRQHandler:
INTMTEMG2_IRQHandler:
INTMTPTB20_IRQHandler:
INTMTTTB21_IRQHandler:
INTMTCAP20_IRQHandler:
INTMTCAP21_IRQHandler:
INTMTEMG3_IRQHandler:
INTMTPTB30_IRQHandler:
INTMTTTB31_IRQHandler:
INTMTCAP30_IRQHandler:
INTMTCAP31_IRQHandler:
INTRMCRX_IRQHandler:
INTTB0_IRQHandler:
INTCAP00_IRQHandler:
INTCAP01_IRQHandler:
INTTB1_IRQHandler:
INTCAP10_IRQHandler:
INTCAP11_IRQHandler:
INTTB2_IRQHandler:
INTCAP20_IRQHandler:
INTCAP21_IRQHandler:
INTTB3_IRQHandler:
INTCAP30_IRQHandler:
INTCAP31_IRQHandler:
INTTB4_IRQHandler:
INTCAP40_IRQHandler:
INTCAP41_IRQHandler:
INTTB5_IRQHandler:
INTCAP50_IRQHandler:
INTCAP51_IRQHandler:
INTTB6_IRQHandler:
INTCAP60_IRQHandler:
INTCAP61_IRQHandler:
INTTB7_IRQHandler:
INTCAP70_IRQHandler:
INTCAP71_IRQHandler:
INTRTC_IRQHandler:
INTDMAADA_IRQHandler:
INTDMAADB_IRQHandler:
INTDMADAA_IRQHandler:
INTDMADAB_IRQHandler:
INTDMASPR0_IRQHandler:
INTDMASPT0_IRQHandler:
INTDMASPR1_IRQHandler:
INTDMASPT1_IRQHandler:
INTDMASPR2_IRQHandler:
INTDMASPT2_IRQHandler:
INTDMAUTR4_IRQHandler:
INTDMAUTT4_IRQHandler:
INTDMAUTR5_IRQHandler:
INTDMAUTT5_IRQHandler:
INTDMARX0_IRQHandler:
INTDMATX0_IRQHandler:
INTDMARX1_IRQHandler:
INTDMATX1_IRQHandler:
INTDMARX2_IRQHandler:
INTDMATX2_IRQHandler:
INTDMARX3_IRQHandler:
INTDMATX3_IRQHandler:
INTDMASBI1_IRQHandler:
INTDMASBI2_IRQHandler:
INTDMATB_IRQHandler:
INTDMARQ_IRQHandler:
INTDMAAERR_IRQHandler:
INTDMABERR_IRQHandler:

        B       .

        .end




