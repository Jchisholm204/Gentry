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
File    : startup_TM4C129.s
Purpose : Startup code and vector table for TI TM4C129 devices
--------  END-OF-HEADER  ---------------------------------------------
*/

  .syntax unified
    .cpu cortex-m4
    .fpu softvfp
    .thumb

.global  g_pfnVectors
.global Default_Handler
.global __Vectors

/* start address for the initialization values of the .data section.
defined in linker script */
.word   _sidata
/* start address for the .data section. defined in linker script */
.word   _sdata
/* end address for the .data section. defined in linker script */
.word   _edata
/* start address for the .bss section. defined in linker script */
.word   _sbss
/* end address for the .bss section. defined in linker script */
.word   _ebss

    .section    .text.Reset_Handler
    .weak   Reset_Handler
    .type   Reset_Handler, %function
Reset_Handler:
/* Setup stack pointer. Helpful for targets running in RAM without script file */
    ldr   r1, =__stack_end__
    msr   msp, r1

/* Call the clock system intitialization function.*/
    bl  SystemInit

/* Copy the data segment initializers from flash to SRAM */
    movs    r1, #0
    b   LoopCopyDataInit

CopyDataInit:
    ldr r3, =_sidata
    ldr r3, [r3, r1]
    str r3, [r0, r1]
    adds    r1, r1, #4

LoopCopyDataInit:
    ldr r0, =_sdata
    ldr r3, =_edata
    adds    r2, r0, r1
    cmp r2, r3
    bcc CopyDataInit
    ldr r2, =_sbss
    b   LoopFillZerobss
/* Zero fill the bss segment. */
FillZerobss:
    movs    r3, #0
    str r3, [r2], #4

LoopFillZerobss:
    ldr r3, = _ebss
    cmp r2, r3
    bcc FillZerobss

/* FPU settings */
    ldr     r0, =0xE000ED88
    ldr     r1,[r0]
    orr     r1,r1,#(0xF << 20)
    str     r1,[r0]


/* Call static constructors */
   /* bl __libc_init_array */
/* Call the application's entry point.*/
    bl  main
/* Branch LoopForever */
LoopForever:
    b LoopForever

.size   Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
    .section    .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
    b   Infinite_Loop
    .size   Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M3.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
    .section    .vectors,"a",%progbits
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors


g_pfnVectors:
__Vectors:
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
    .word   GPIOA_Handler             /*   0: GPIO Port A                   */
    .word   GPIOB_Handler             /*   1: GPIO Port B                   */
    .word   GPIOC_Handler             /*   2: GPIO Port C                   */
    .word   GPIOD_Handler             /*   3: GPIO Port D                   */
    .word   GPIOE_Handler             /*   4: GPIO Port E                   */
    .word   UART0_Handler             /*   5: UART0 Rx and Tx               */
    .word   UART1_Handler             /*   6: UART1 Rx and Tx               */
    .word   SSI0_Handler              /*   7: SSI0 Rx and Tx                */
    .word   I2C0_Handler              /*  8: I2C0 Master and Slave          */
    .word   PMW0_FAULT_Handler        /*   9: PWM Fault                     */
    .word   PWM0_0_Handler            /*  10: PWM Generator 0               */
    .word   PWM0_1_Handler            /*  11: PWM Generator 1               */
    .word   PWM0_2_Handler            /*  12: PWM Generator 2               */
    .word   QEI0_Handler              /*  13: Quadrature Encoder 0          */
    .word   ADC0SS0_Handler           /*  14: ADC Sequence 0                */
    .word   ADC0SS1_Handler           /*  15: ADC Sequence 1                */
    .word   ADC0SS2_Handler           /*  16: ADC Sequence 2                */
    .word   ADC0SS3_Handler           /*  17: ADC Sequence 3                */
    .word   WDT0_Handler              /*  18: Watchdog timer                */
    .word   TIMER0A_Handler           /*  19: Timer 0 subtimer A            */
    .word   TIMER0B_Handler           /*  20: Timer 0 subtimer B            */
    .word   TIMER1A_Handler           /*  21: Timer 1 subtimer A            */
    .word   TIMER1B_Handler           /*  22: Timer 1 subtimer B            */
    .word   TIMER2A_Handler           /*  23: Timer 2 subtimer A            */
    .word   TIMER2B_Handler           /*  24: Timer 2 subtimer B            */
    .word   COMP0_Handler             /*  25: Analog Comparator 0           */
    .word   COMP1_Handler             /*  26: Analog Comparator 1           */
    .word   COMP2_Handler             /*  27: Analog Comparator 2           */
    .word   SYSCTL_Handler            /*  28: System Control (PLL, OSC, BO) */
    .word   FLASH_Handler             /*  29: FLASH Control                 */
    .word   GPIOF_Handler             /*  30: GPIO Port F                   */
    .word   GPIOG_Handler             /*  31: GPIO Port G                   */
    .word   GPIOH_Handler             /*  32: GPIO Port H                   */
    .word   UART2_Handler             /*  33: UART2 Rx and Tx               */
    .word   SSI1_Handler              /*  34: SSI1 Rx and Tx                */
    .word   TIMER3A_Handler           /*  35: Timer 3 subtimer A            */
    .word   TIMER3B_Handler           /*  36: Timer 3 subtimer B            */
    .word   I2C1_Handler              /*  37: I2C1 Master and Slave         */
    .word   CAN0_Handler              /*  38: CAN0                          */
    .word   CAN1_Handler              /*  39: CAN1                          */
    .word   ETH_Handler               /*  40: Ethernet                      */
    .word   HIB_Handler               /*  41: Hibernate                     */
    .word   USB0_Handler              /*  42: USB0                          */
    .word   PWM0_3_Handler            /*  43: PWM Generator 3               */
    .word   UDMA_Handler              /*  44: uDMA Software Transfer        */
    .word   UDMAERR_Handler           /*  45: uDMA Error                    */
    .word   ADC1SS0_Handler           /*  46: ADC1 Sequence 0               */
    .word   ADC1SS1_Handler           /*  47: ADC1 Sequence 1               */
    .word   ADC1SS2_Handler           /*  48: ADC1 Sequence 2               */
    .word   ADC1SS3_Handler           /*  49: ADC1 Sequence 3               */
    .word   EBI0_Handler              /*  50: External Bus Interface 0      */
    .word   GPIOJ_Handler             /*  51: GPIO Port J                   */
    .word   GPIOK_Handler             /*  52: GPIO Port K                   */
    .word   GPIOL_Handler             /*  53: GPIO Port L                   */
    .word   SSI2_Handler              /*  54: SSI2 Rx and Tx                */
    .word   SSI3_Handler              /*  55: SSI3 Rx and Tx                */
    .word   UART3_Handler             /*  56: UART3 Rx and Tx               */
    .word   UART4_Handler             /*  57: UART4 Rx and Tx               */
    .word   UART5_Handler             /*  58: UART5 Rx and Tx               */
    .word   UART6_Handler             /*  59: UART6 Rx and Tx               */
    .word   UART7_Handler             /*  60: UART7 Rx and Tx               */
    .word   I2C2_Handler              /*  61: I2C2 Master and Slave         */
    .word   I2C3_Handler              /*  62: I2C3 Master and Slave         */
    .word   TIMER4A_Handler           /*  63: Timer 4 subtimer A            */
    .word   TIMER4B_Handler           /*  64: Timer 4 subtimer B            */
    .word   TIMER5A_Handler           /*  65: Timer 5 subtimer A            */
    .word   TIMER5B_Handler           /*  66: Timer 5 subtimer B            */
    .word   FPU_Handler               /*  67: FPU                           */
    .word   0                         /*  68: Reserved                      */
    .word   0                         /*  69: Reserved                      */
    .word   I2C4_Handler              /*  70: I2C4 Master and Slave         */
    .word   I2C5_Handler              /*  71: I2C5 Master and Slave         */
    .word   GPIOM_Handler             /*  72: GPIO Port M                   */
    .word   GPION_Handler             /*  73: GPIO Port N                   */
    .word   0                         /*  74: Reserved                      */
    .word   TAMPER_Handler            /*  75: Tamper                        */
    .word   GPIOP0_Handler            /*  76: GPIO Port P (Summary or P0)   */
    .word   GPIOP1_Handler            /*  77: GPIO Port P1                  */
    .word   GPIOP2_Handler            /*  78: GPIO Port P2                  */
    .word   GPIOP3_Handler            /*  79: GPIO Port P3                  */
    .word   GPIOP4_Handler            /*  80: GPIO Port P4                  */
    .word   GPIOP5_Handler            /*  81: GPIO Port P5                  */
    .word   GPIOP6_Handler            /*  82: GPIO Port P6                  */
    .word   GPIOP7_Handler            /*  83: GPIO Port P7                  */
    .word   GPIOQ0_Handler            /*  84: GPIO Port Q (Summary or Q0)   */
    .word   GPIOQ1_Handler            /*  85: GPIO Port Q1                  */
    .word   GPIOQ2_Handler            /*  86: GPIO Port Q2                  */
    .word   GPIOQ3_Handler            /*  87: GPIO Port Q3                  */
    .word   GPIOQ4_Handler            /*  88: GPIO Port Q4                  */
    .word   GPIOQ5_Handler            /*  89: GPIO Port Q5                  */
    .word   GPIOQ6_Handler            /*  90: GPIO Port Q6                  */
    .word   GPIOQ7_Handler            /*  91: GPIO Port Q7                  */
    .word   GPIOR_Handler             /*  92: GPIO Port R                   */
    .word   GPIOS_Handler             /*  93: GPIO Port S                   */
    .word   SHAMD5_Handler            /*  94: SHA/MD5 0                     */
    .word   AES_Handler               /*  95: AES 0                         */
    .word   DES3DES_Handler           /*  96: DES3DES 0                     */
    .word   LCDCONTROLLER_Handler     /*  97: LCD Controller 0              */
    .word   TIMER6A_Handler           /*  98: Timer 6 subtimer A            */
    .word   TIMER6B_Handler           /*  99: Timer 6 subtimer B            */
    .word   TIMER7A_Handler           /* 100: Timer 7 subtimer A            */
    .word   TIMER7B_Handler           /* 101: Timer 7 subtimer B            */
    .word   I2C6_Handler              /* 102: I2C6 Master and Slave         */
    .word   I2C7_Handler              /* 103: I2C7 Master and Slave         */
    .word   HIMSCANKEYBOARD_Handler   /* 104: HIM Scan Matrix Keyboard 0    */
    .word   ONEWIRE_Handler           /* 105: One Wire 0                    */
    .word   HIMPS2_Handler            /* 106: HIM PS/2 0                    */
    .word   HIMLEDSEQUENCER_Handler   /* 107: HIM LED Sequencer 0           */
    .word   HIMCONSUMERIR_Handler     /* 108: HIM Consumer IR 0             */
    .word   I2C8_Handler              /* 109: I2C8 Master and Slave         */
    .word   I2C9_Handler              /* 110: I2C9 Master and Slave         */
    .word   GPIOT_Handler             /* 111: GPIO Port T                   */

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

    .weak NMI_Handler
    .thumb_set NMI_Handler,Default_Handler

    .weak HardFault_Handler
    .thumb_set HardFault_Handler,Default_Handler

    .weak MemManage_Handler
    .thumb_set MemManage_Handler,Default_Handler

    .weak BusFault_Handler
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

    .weak   WWDG_IRQHandler
    .thumb_set WWDG_IRQHandler,Default_Handler



    .weak   GPIOA_Handler
    .thumb_set GPIOA_Handler,Default_Handler

    .weak   GPIOB_Handler
    .thumb_set GPIOB_Handler,Default_Handler

    .weak   GPIOC_Handler
    .thumb_set GPIOC_Handler,Default_Handler

    .weak   GPIOD_Handler
    .thumb_set GPIOD_Handler,Default_Handler

    .weak   GPIOE_Handler
    .thumb_set GPIOE_Handler,Default_Handler

    .weak   UART0_Handler
    .thumb_set UART0_Handler,Default_Handler

    .weak   UART1_Handler
    .thumb_set UART1_Handler,Default_Handler

    .weak   SSI0_Handler
    .thumb_set SSI0_Handler,Default_Handler

    .weak   I2C0_Handler
    .thumb_set I2C0_Handler,Default_Handler

    .weak   PMW0_FAULT_Handler
    .thumb_set PMW0_FAULT_Handler,Default_Handler

    .weak   PWM0_0_Handler
    .thumb_set PWM0_0_Handler,Default_Handler

    .weak   PWM0_1_Handler
    .thumb_set PWM0_1_Handler,Default_Handler

    .weak   PWM0_2_Handler
    .thumb_set PWM0_2_Handler,Default_Handler

    .weak   QEI0_Handler
    .thumb_set QEI0_Handler,Default_Handler

    .weak   ADC0SS0_Handler
    .thumb_set ADC0SS0_Handler,Default_Handler

    .weak   ADC0SS1_Handler
    .thumb_set ADC0SS1_Handler,Default_Handler

    .weak   ADC0SS2_Handler
    .thumb_set ADC0SS2_Handler,Default_Handler

    .weak   ADC0SS3_Handler
    .thumb_set ADC0SS3_Handler,Default_Handler

    .weak   WDT0_Handler
    .thumb_set WDT0_Handler,Default_Handler

    .weak   TIMER0A_Handler
    .thumb_set TIMER0A_Handler,Default_Handler

    .weak   TIMER0B_Handler
    .thumb_set TIMER0B_Handler,Default_Handler

    .weak   TIMER1A_Handler
    .thumb_set TIMER1A_Handler,Default_Handler

    .weak   TIMER1B_Handler
    .thumb_set TIMER1B_Handler,Default_Handler

    .weak   TIMER2A_Handler
    .thumb_set TIMER2A_Handler,Default_Handler

    .weak   TIMER2B_Handler
    .thumb_set TIMER2B_Handler,Default_Handler

    .weak   COMP0_Handler
    .thumb_set COMP0_Handler,Default_Handler

    .weak   COMP1_Handler
    .thumb_set COMP1_Handler,Default_Handler

    .weak   COMP2_Handler
    .thumb_set COMP2_Handler,Default_Handler

    .weak   SYSCTL_Handler
    .thumb_set SYSCTL_Handler,Default_Handler

    .weak   FLASH_Handler
    .thumb_set FLASH_Handler,Default_Handler

    .weak   GPIOF_Handler
    .thumb_set GPIOF_Handler,Default_Handler

    .weak   GPIOG_Handler
    .thumb_set GPIOG_Handler,Default_Handler

    .weak   GPIOH_Handler
    .thumb_set GPIOH_Handler,Default_Handler

    .weak   UART2_Handler
    .thumb_set UART2_Handler,Default_Handler

    .weak   SSI1_Handler
    .thumb_set SSI1_Handler,Default_Handler

    .weak   TIMER3A_Handler
    .thumb_set TIMER3A_Handler,Default_Handler

    .weak   TIMER3B_Handler
    .thumb_set TIMER3B_Handler,Default_Handler

    .weak   I2C1_Handler
    .thumb_set I2C1_Handler,Default_Handler

    .weak   CAN0_Handler
    .thumb_set CAN0_Handler,Default_Handler

    .weak   CAN1_Handler
    .thumb_set CAN1_Handler,Default_Handler

    .weak   ETH_Handler
    .thumb_set ETH_Handler,Default_Handler

    .weak   HIB_Handler
    .thumb_set HIB_Handler,Default_Handler

    .weak   USB0_Handler
    .thumb_set USB0_Handler,Default_Handler

    .weak   PWM0_3_Handler
    .thumb_set PWM0_3_Handler,Default_Handler

    .weak   UDMA_Handler
    .thumb_set UDMA_Handler,Default_Handler

    .weak   UDMAERR_Handler
    .thumb_set UDMAERR_Handler,Default_Handler

    .weak   ADC1SS0_Handler
    .thumb_set ADC1SS0_Handler,Default_Handler

    .weak   ADC1SS1_Handler
    .thumb_set ADC1SS1_Handler,Default_Handler

    .weak   ADC1SS2_Handler
    .thumb_set ADC1SS2_Handler,Default_Handler

    .weak   ADC1SS3_Handler
    .thumb_set ADC1SS3_Handler,Default_Handler

    .weak   EBI0_Handler
    .thumb_set EBI0_Handler,Default_Handler

    .weak   GPIOJ_Handler
    .thumb_set GPIOJ_Handler,Default_Handler

    .weak   GPIOK_Handler
    .thumb_set GPIOK_Handler,Default_Handler

    .weak   GPIOL_Handler
    .thumb_set GPIOL_Handler,Default_Handler

    .weak   SSI2_Handler
    .thumb_set SSI2_Handler,Default_Handler

    .weak   SSI3_Handler
    .thumb_set SSI3_Handler,Default_Handler

    .weak   UART3_Handler
    .thumb_set UART3_Handler,Default_Handler

    .weak   UART4_Handler
    .thumb_set UART4_Handler,Default_Handler

    .weak   UART5_Handler
    .thumb_set UART5_Handler,Default_Handler

    .weak   UART6_Handler
    .thumb_set UART6_Handler,Default_Handler

    .weak   UART7_Handler
    .thumb_set UART7_Handler,Default_Handler

    .weak   I2C2_Handler
    .thumb_set I2C2_Handler,Default_Handler

    .weak   I2C3_Handler
    .thumb_set I2C3_Handler,Default_Handler

    .weak   TIMER4A_Handler
    .thumb_set TIMER4A_Handler,Default_Handler

    .weak   TIMER4B_Handler
    .thumb_set TIMER4B_Handler,Default_Handler

    .weak   TIMER5A_Handler
    .thumb_set TIMER5A_Handler,Default_Handler

    .weak   TIMER5B_Handler
    .thumb_set TIMER5B_Handler,Default_Handler

    .weak   FPU_Handler
    .thumb_set FPU_Handler,Default_Handler

    .weak   I2C4_Handler
    .thumb_set I2C4_Handler,Default_Handler

    .weak   I2C5_Handler
    .thumb_set I2C5_Handler,Default_Handler

    .weak   GPIOM_Handler
    .thumb_set GPIOM_Handler,Default_Handler

    .weak   GPION_Handler
    .thumb_set GPION_Handler,Default_Handler

    .weak   TAMPER_Handler
    .thumb_set TAMPER_Handler,Default_Handler

    .weak   GPIOP0_Handler
    .thumb_set GPIOP0_Handler,Default_Handler

    .weak   GPIOP1_Handler
    .thumb_set GPIOP1_Handler,Default_Handler

    .weak   GPIOP2_Handler
    .thumb_set GPIOP2_Handler,Default_Handler

    .weak   GPIOP3_Handler
    .thumb_set GPIOP3_Handler,Default_Handler

    .weak   GPIOP4_Handler
    .thumb_set GPIOP4_Handler,Default_Handler

    .weak   GPIOP5_Handler
    .thumb_set GPIOP5_Handler,Default_Handler

    .weak   GPIOP6_Handler
    .thumb_set GPIOP6_Handler,Default_Handler

    .weak   GPIOP7_Handler
    .thumb_set GPIOP7_Handler,Default_Handler

    .weak   GPIOQ0_Handler
    .thumb_set GPIOQ0_Handler,Default_Handler

    .weak   GPIOQ1_Handler
    .thumb_set GPIOQ1_Handler,Default_Handler

    .weak   GPIOQ2_Handler
    .thumb_set GPIOQ2_Handler,Default_Handler

    .weak   GPIOQ3_Handler
    .thumb_set GPIOQ3_Handler,Default_Handler

    .weak   GPIOQ4_Handler
    .thumb_set GPIOQ4_Handler,Default_Handler

    .weak   GPIOQ5_Handler
    .thumb_set GPIOQ5_Handler,Default_Handler

    .weak   GPIOQ6_Handler
    .thumb_set GPIOQ6_Handler,Default_Handler

    .weak   GPIOQ7_Handler
    .thumb_set GPIOQ7_Handler,Default_Handler

    .weak   GPIOR_Handler
    .thumb_set GPIOR_Handler,Default_Handler

    .weak   GPIOS_Handler
    .thumb_set GPIOS_Handler,Default_Handler

    .weak   SHAMD5_Handler
    .thumb_set SHAMD5_Handler,Default_Handler

    .weak   AES_Handler
    .thumb_set AES_Handler,Default_Handler

    .weak   DES3DES_Handler
    .thumb_set DES3DES_Handler,Default_Handler

    .weak   LCDCONTROLLER_Handler
    .thumb_set LCDCONTROLLER_Handler,Default_Handler

    .weak   TIMER6A_Handler
    .thumb_set TIMER6A_Handler,Default_Handler

    .weak   TIMER6B_Handler
    .thumb_set TIMER6B_Handler,Default_Handler

    .weak   TIMER7A_Handler
    .thumb_set TIMER7A_Handler,Default_Handler

    .weak   TIMER7B_Handler
    .thumb_set TIMER7B_Handler,Default_Handler

    .weak   I2C6_Handler
    .thumb_set I2C6_Handler,Default_Handler

    .weak   I2C7_Handler
    .thumb_set I2C7_Handler,Default_Handler

    .weak   HIMSCANKEYBOARD_Handler
    .thumb_set HIMSCANKEYBOARD_Handler,Default_Handler

    .weak   ONEWIRE_Handler
    .thumb_set ONEWIRE_Handler,Default_Handler

    .weak   HIMPS2_Handler
    .thumb_set HIMPS2_Handler,Default_Handler

    .weak   HIMLEDSEQUENCER_Handler
    .thumb_set HIMLEDSEQUENCER_Handler,Default_Handler

    .weak   HIMCONSUMERIR_Handler
    .thumb_set HIMCONSUMERIR_Handler,Default_Handler

    .weak   I2C8_Handler
    .thumb_set I2C8_Handler,Default_Handler

    .weak   I2C9_Handler
    .thumb_set I2C9_Handler,Default_Handler

    .weak   GPIOT_Handler
    .thumb_set GPIOT_Handler,Default_Handler
