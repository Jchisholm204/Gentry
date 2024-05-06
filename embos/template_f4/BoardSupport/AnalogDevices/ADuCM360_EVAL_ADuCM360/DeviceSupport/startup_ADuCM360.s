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
File    : startup_ADuCM360.s
Purpose : ADuCM360 tartup code and vector table
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

    /* External Interrupts */
	  .word   WakeUp_Int_Handler        /* Wake Up Timer              [ 0] */
	  .word   Ext_Int0_Handler          /* External Interrupt 0       [ 1] */
	  .word   Ext_Int1_Handler          /* External Interrupt 1       [ 2] */
	  .word   Ext_Int2_Handler          /* External Interrupt 2       [ 3] */
	  .word   Ext_Int3_Handler          /* External Interrupt 3       [ 4] */
	  .word   Ext_Int4_Handler          /* External Interrupt 4       [ 5] */
	  .word   Ext_Int5_Handler          /* External Interrupt 5       [ 6] */
	  .word   Ext_Int6_Handler          /* External Interrupt 6       [ 7] */
	  .word   Ext_Int7_Handler          /* External Interrupt 7       [ 8] */
	  .word   WDog_Tmr_Int_Handler      /* Watchdog timer handler     [ 9] */
	  .word   0                         /*                            [10] */
	  .word   GP_Tmr0_Int_Handler       /* General purpose timer 0    [11] */
	  .word   GP_Tmr1_Int_Handler       /* General purpose timer 1    [12] */
	  .word   ADC0_Int_Handler          /* ADC0 Interrupt             [13] */
	  .word   ADC1_Int_Handler          /* ADC1 Interrupt             [14] */
	  .word   SINC2_Int_Handler         /* SINC2 Interrupt            [15] */
	  .word   Flsh_Int_Handler          /* Flash Interrupt            [16] */
	  .word   UART_Int_Handler          /* UART0                      [17] */
	  .word   SPI0_Int_Handler          /* SPI 0                      [18] */
	  .word   SPI1_Int_Handler          /* SPI 1                      [19] */
	  .word   I2C0_Slave_Int_Handler    /* I2C0 Slave                 [20] */
	  .word   I2C0_Master_Int_Handler   /* I2C0 Master                [21] */
	  .word   DMA_Err_Int_Handler       /* DMA Error interrupt        [22] */
	  .word   DMA_SPI1_TX_Int_Handler   /* DMA SPI1 TX                [23] */
	  .word   DMA_SPI1_RX_Int_Handler   /* DMA SPI1 RX                [24] */
	  .word   DMA_UART_TX_Int_Handler   /* DMA UART TX                [25] */
	  .word   DMA_UART_RX_Int_Handler   /* DMA UART RX                [26] */
	  .word   DMA_I2C0_STX_Int_Handler  /* DMA I2C0 Slave TX          [27] */
	  .word   DMA_I2C0_SRX_Int_Handler  /* DMA I2C0 Slave RX          [28] */
	  .word   DMA_I2C0_MTX_Int_Handler  /* DMA I2C0 Master TX         [29] */
	  .word   DMA_I2C0_MRX_Int_Handler  /* DMA I2C0 Master RX         [30] */
	  .word   DMA_DAC_Out_Int_Handler   /* DMA DAC out                [31] */
	  .word   DMA_ADC0_Int_Handler      /* DMA ADC0                   [32] */
	  .word   DMA_ADC1_Int_Handler      /* DMA ADC1                   [33] */
	  .word   DMA_SINC2_Int_Handler     /* SINC2                      [34] */
	  .word   PWMTRIP_Int_Handler       /* PWMTRIP                    [35] */
	  .word   PWM0_Int_Handler          /* PWM0                       [36] */
	  .word   PWM1_Int_Handler          /* PWM1                       [37] */
	  .word   PWM2_Int_Handler          /* PWM2                       [38] */
	  .word   0                         /*                            [39] */

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

    .weak Ext_Int0_Handler
    .thumb_set Ext_Int0_Handler,Default_Handler

    .weak WakeUp_Int_Handler
    .thumb_set WakeUp_Int_Handler,Default_Handler

    .weak Ext_Int1_Handler
    .thumb_set Ext_Int1_Handler,Default_Handler

    .weak Ext_Int2_Handler
    .thumb_set Ext_Int2_Handler,Default_Handler

    .weak Ext_Int3_Handler
    .thumb_set Ext_Int3_Handler,Default_Handler

    .weak Ext_Int4_Handler
    .thumb_set Ext_Int4_Handler,Default_Handler

    .weak Ext_Int5_Handler
    .thumb_set Ext_Int5_Handler,Default_Handler

    .weak Ext_Int6_Handler
    .thumb_set Ext_Int6_Handler,Default_Handler

    .weak Ext_Int7_Handler
    .thumb_set Ext_Int7_Handler,Default_Handler

    .weak WDog_Tmr_Int_Handler
    .thumb_set WDog_Tmr_Int_Handler,Default_Handler

    .weak GP_Tmr0_Int_Handler
    .thumb_set GP_Tmr0_Int_Handler,Default_Handler

    .weak GP_Tmr1_Int_Handler
    .thumb_set GP_Tmr1_Int_Handler,Default_Handler

    .weak ADC0_Int_Handler
    .thumb_set ADC0_Int_Handler,Default_Handler

    .weak ADC1_Int_Handler
    .thumb_set ADC1_Int_Handler,Default_Handler

    .weak SINC2_Int_Handler
    .thumb_set SINC2_Int_Handler,Default_Handler

    .weak Flsh_Int_Handler
    .thumb_set Flsh_Int_Handler,Default_Handler

    .weak UART_Int_Handler
    .thumb_set UART_Int_Handler,Default_Handler

    .weak SPI0_Int_Handler
    .thumb_set SPI0_Int_Handler,Default_Handler

    .weak SPI1_Int_Handler
    .thumb_set SPI1_Int_Handler,Default_Handler

    .weak I2C0_Slave_Int_Handler
    .thumb_set I2C0_Slave_Int_Handler,Default_Handler

    .weak I2C0_Master_Int_Handler
    .thumb_set I2C0_Master_Int_Handler,Default_Handler

    .weak DMA_Err_Int_Handler
    .thumb_set DMA_Err_Int_Handler,Default_Handler

    .weak DMA_SPI1_TX_Int_Handler
    .thumb_set DMA_SPI1_TX_Int_Handler,Default_Handler

    .weak DMA_SPI1_RX_Int_Handler
    .thumb_set DMA_SPI1_RX_Int_Handler,Default_Handler

    .weak DMA_UART_TX_Int_Handler
    .thumb_set DMA_UART_TX_Int_Handler,Default_Handler

    .weak DMA_UART_RX_Int_Handler
    .thumb_set DMA_UART_RX_Int_Handler,Default_Handler

    .weak DMA_I2C0_STX_Int_Handler
    .thumb_set DMA_I2C0_STX_Int_Handler,Default_Handler

    .weak DMA_I2C0_SRX_Int_Handler
    .thumb_set DMA_I2C0_SRX_Int_Handler,Default_Handler

    .weak DMA_I2C0_MTX_Int_Handler
    .thumb_set DMA_I2C0_MTX_Int_Handler,Default_Handler

    .weak DMA_I2C0_MRX_Int_Handler
    .thumb_set DMA_I2C0_MRX_Int_Handler,Default_Handler

    .weak DMA_DAC_Out_Int_Handler
    .thumb_set DMA_DAC_Out_Int_Handler,Default_Handler

    .weak DMA_ADC0_Int_Handler
    .thumb_set DMA_ADC0_Int_Handler,Default_Handler

    .weak DMA_ADC1_Int_Handler
    .thumb_set DMA_ADC1_Int_Handler,Default_Handler

    .weak DMA_SINC2_Int_Handler
    .thumb_set DMA_SINC2_Int_Handler,Default_Handler

    .weak PWMTRIP_Int_Handler
    .thumb_set PWMTRIP_Int_Handler,Default_Handler

    .weak PWM0_Int_Handler
    .thumb_set PWM0_Int_Handler,Default_Handler

    .weak PWM1_Int_Handler
    .thumb_set PWM1_Int_Handler,Default_Handler

    .weak PWM2_Int_Handler
    .thumb_set PWM2_Int_Handler,Default_Handler

        .end
