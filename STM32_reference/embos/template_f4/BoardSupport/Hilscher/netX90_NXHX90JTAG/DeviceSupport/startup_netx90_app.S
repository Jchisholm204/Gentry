/**************************************************************************//**
 * @file     startup_netx90_app.S
 * @brief    CMSIS Cortex-M4 Core Device Startup File for
 *           ARMCM4 Device netx90_app
 * @version  V5.00
 * @date     02. March 2016
 ******************************************************************************/
/*
 * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


	.syntax	unified
    .cpu cortex-m4
	.arch	armv7-m

	.section .stack
	.align	3
#ifdef __STACK_SIZE
	.equ	Stack_Size, __STACK_SIZE
#else
	.equ	Stack_Size, 0x00002000
#endif
	.globl	__StackTop
	.globl	__StackLimit
__StackLimit:
	.space	Stack_Size
	.size	__StackLimit, . - __StackLimit
__StackTop:
	.size	__StackTop, . - __StackTop

	.section .heap
	.align	3
#ifdef __HEAP_SIZE
	.equ	Heap_Size, __HEAP_SIZE
#else
	.equ	Heap_Size, 0x00002000
#endif
	.globl	__HeapBase
	.globl	__HeapLimit
__HeapBase:
	.if	Heap_Size
	.space	Heap_Size
	.endif
	.size	__HeapBase, . - __HeapBase
__HeapLimit:
	.size	__HeapLimit, . - __HeapLimit

	.section .vectors
	.align	2
	.globl	__Vectors
    .globl  __Vectors_Table
__Vectors:
__Vectors_Table:
	.long	__StackTop            /* Top of Stack */
	.long	Reset_Handler         /* Reset Handler */
	.long	NMI_Handler           /* NMI Handler */
	.long	HardFault_Handler     /* Hard Fault Handler */
	.long	MemManage_Handler     /* MPU Fault Handler */
	.long	BusFault_Handler      /* Bus Fault Handler */
	.long	UsageFault_Handler    /* Usage Fault Handler */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	SVC_Handler           /* SVCall Handler */
	.long	DebugMon_Handler      /* Debug Monitor Handler */
	.long	0                     /* Reserved */
	.long	PendSV_Handler        /* PendSV Handler */
	.long	SysTick_Handler       /* SysTick Handler */

	/* External interrupts */
	.long	SW_IRQHandler         /*  0: software  */
	.long	TIM0_IRQHandler       /*  1: timer_app0 */
	.long	TIM1_IRQHandler       /*  2: timer_app1 */
	.long	TIM2_IRQHandler       /*  3: timer_app2 */
	.long	SYSTIME_IRQHandler    /*  4: timer_app_systime_s */
	.long	WDG_IRQHandler        /*  5: wdg_app */
	.long	DMAC_IRQHandler       /*  6: dmac_app */
	.long	MCP_IRQHandler        /*  7: mcp_app */
	.long	UART_IRQHandler       /*  8: uart */
	.long	I2C_IRQHandler        /*  9: i2c-app */
	.long	I2CXPIC_IRQHandler    /* 10: i2c_xpic_app */
	.long	ECC_IRQHandler        /* 11: ecc_app_1bit_error */
	.long	XPICDEBUG_IRQHandler  /* 12: xpic_debug_app */
	.long	WDGXPIC_IRQHandler    /* 13: wdg_xpic_app_arm */
	.long	NFIFO_ARM_APP_IRQHandler/* 14: nfifo_arm_app */
	.long	IOLINK_IRQHandler     /* 15: io_link_irq_xpic_app */
	.long	SPI0_IRQHandler       /* 16: spi0_app */
	.long	SPI1_IRQHandler       /* 17: spi1_app */
	.long	SPI2_IRQHandler       /* 18: spi2_app */
	.long	SPIXPIC_IRQHandler    /* 19: spi_xpic_app */
	.long	UARTAPP_IRQHandler    /* 20: uart_appp */
	.long	UARTXPIC_IRQHandler   /* 21: uart_xpic_app */
	.long	ASIC_IRQHandler       /* 22: bod */
	.long	CLKSUP_IRQHandler     /* 23: clksup_app */
	.long	SQI_IRQHandler        /* 24: sqi */
	.long	HIFPIO_IRQHandler     /* 25: hif_pio_arm */
	.long	ETH_IRQHandler        /* 26: eth */
	.long	HASH_IRQHandler       /* 27: hash */
	.long	AES_IRQHandler        /* 28: aes */
	.long	MTGY_IRQHandler       /* 29: mtgy */
	.long	HIFRDY_IRQHandler     /* 30: hif_rdy_to */
	.long	IDPM_IRQHandler       /* 31: idpm_com_host */
	.long	HSC0_IRQHandler       /* 32: hanshake channel 0 */
	.long	HSC1_IRQHandler       /* 33: hanshake channel 1 */
	.long	HSC2_IRQHandler       /* 34: hanshake channel 2 */
	.long	HSC3_IRQHandler       /* 35: hanshake channel 3 */
	.long	HSC4_IRQHandler       /* 36: hanshake channel 4 */
	.long	HSC5_IRQHandler       /* 37: hanshake channel 5 */
	.long	HSC6_IRQHandler       /* 38: hanshake channel 6 */
	.long	HSC7_IRQHandler       /* 39: hanshake channel 7 */
	.long	HSC8TO15_IRQHandler   /* 40: hanshake channel 8 to 15 */
	.long	ENDAT1_IRQHandler     /* 41: endat_app0 */
	.long	ENDAT2_IRQHandler     /* 42: endat_app1 */
	.long	BISS0_IRQHandler      /* 43: biss_app0 */
	.long	BISS1_IRQHandler      /* 44: biss_app1 */
	.long	MADC0_IRQHandler      /* 45: madc_seq0 */
	.long	MADC1_IRQHandler      /* 46: madc_seq1 */
	.long	MADC2_IRQHandler      /* 47: madc_seq2 */
	.long	MADC3_IRQHandler      /* 48: madc_seq3 */
	.long	MPWM_IRQHandler       /* 49: mpwm */
	.long	MENC0_IRQHandler      /* 50: menc_app_irq_enc0 */
	.long	MENC1_IRQHandler      /* 51: menc_app_irq_enc1 */
	.long	MENCCAP_IRQHandler    /* 52: menc_app_irq_cap_mp */
	.long	MENCERR_IRQHandler    /* 53: menc_app_irq_err */
	.long	GPIO0_IRQHandler      /* 54: gpio_app0 */
	.long	GPIO1_IRQHandler      /* 55: gpio_app1 */
	.long	GPIO2_IRQHandler      /* 56: gpio_app2 */
	.long	GPIO3_IRQHandler      /* 57: gpio_app3 */
	.long	GPIO4_IRQHandler      /* 58: gpio_app4 */
	.long	GPIO5_IRQHandler      /* 59: gpio_app5 */
	.long	GPIO6_IRQHandler      /* 60: gpio_app6 */
	.long	GPIO7_IRQHandler      /* 61: gpio_app7 */
	.long	GPIOTIM0_IRQHandler   /* 62: gpio_app_timer0 */
	.long	GPIOTIM1_IRQHandler   /* 63: gpio_app_timer1 */
	.long	GPIOTIM2_IRQHandler   /* 64: gpio_app_timer2 */
	.long	PIO0_IRQHandler       /* 65: pio0_app */
	.long	PIO1_IRQHandler       /* 66: pio1_app */
	.long	PIO2_IRQHandler       /* 67: pio2_app */
	.long	PIO3_IRQHandler       /* 68: pio3_app */
	.long	XCTRIGGER0_IRQHandler /* 69: trigger_out_edge0 */
	.long	XCTRIGGER1_IRQHandler /* 70: trigger_out_edge1 */
	.long	CAN0_IRQHandler       /* 71: can_ctrl0_app */
	.long	CAN1_IRQHandler       /* 72: can_ctrl1_app */
	.long	SQI0_IRQHandler       /* 73: sqi0_app */
	.long	SQI1_IRQHandler       /* 74: sqi1_app */
	.long	CTI0_IRQHandler       /* 75: cti0_arm_app */
	.long	CTI1_IRQHandler       /* 76: cti1_arm_app */
	.long	FPU_IRQHandler        /* 77: fpu_arm_app */
    .long   0                     /* 78: reserved  */
    .long   0                     /* 79: reserved  */
    .long   0                     /* 80: reserved  */
    .long   0                     /* 81: reserved  */
    .long   0                     /* 82: reserved  */
    .long   0                     /* 83: reserved  */
    .long   0                     /* 84: reserved  */
    .long   0                     /* 85: reserved  */
    .long   0                     /* 86: reserved  */
    .long   0                     /* 87: reserved  */
    .long   0                     /* 88: reserved  */
    .long   0                     /* 89: reserved  */
    .long   0                     /* 90: reserved  */
    .long   0                     /* 91: reserved  */
    .long   0                     /* 92: reserved  */
    .long   0                     /* 93: reserved  */
    .long   0                     /* 94: reserved  */
    .long   0                     /* 95: reserved  */

	.size	__Vectors, . - __Vectors

	.text
	.thumb
	.thumb_func
	.align	2
	.globl	Reset_Handler
	.type	Reset_Handler, %function
Reset_Handler:
/*  The following define is used to generate an application dummy.     */
#ifdef __GENERATE_DUMMY
    b   .
#else
/*  Firstly it copies data from read only memory to RAM. There are two schemes
 *  to copy. One can copy more than one sections. Another can only copy
 *  one section.  The former scheme needs more instructions and read-only
 *  data to implement than the latter.
 *  Macro __STARTUP_COPY_MULTIPLE is used to choose between two schemes.  */
#ifdef __STARTUP_COPY_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of triplets, each of which specify:
 *    offset 0: LMA of start of a section to copy from
 *    offset 4: VMA of start of a section to copy to
 *    offset 8: size of the section to copy. Must be multiply of 4
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
	ldr	r4, =__copy_table_start__
	ldr	r5, =__copy_table_end__

.L_loop0:
	cmp	r4, r5
	bge	.L_loop0_done
	ldr	r1, [r4]
	ldr	r2, [r4, #4]
	ldr	r3, [r4, #8]

.L_loop0_0:
	subs	r3, #4
	ittt	ge
	ldrge	r0, [r1, r3]
	strge	r0, [r2, r3]
	bge	.L_loop0_0

	adds	r4, #12
	b	.L_loop0

.L_loop0_done:
#else
/*  Single section scheme.
 *
 *  The ranges of copy from/to are specified by following symbols
 *    __etext: LMA of start of the section to copy from. Usually end of text
 *    __data_start__: VMA of start of the section to copy to
 *    __data_end__: VMA of end of the section to copy to
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
	ldr	r1, =__etext
	ldr	r2, =__data_start__
	ldr	r3, =__data_end__

.L_loop1:
	cmp	r2, r3
	ittt	lt
	ldrlt	r0, [r1], #4
	strlt	r0, [r2], #4
	blt	.L_loop1
#endif /*__STARTUP_COPY_MULTIPLE */

/*  This part of work usually is done in C library startup code. Otherwise,
 *  define this macro to enable it in this startup.
 *
 *  There are two schemes too. One can clear multiple BSS sections. Another
 *  can only clear one section. The former is more size expensive than the
 *  latter.
 *
 *  Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 *  Otherwise efine macro __STARTUP_CLEAR_BSS to choose the later.
 */
#ifdef __STARTUP_CLEAR_BSS_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of tuples specifying:
 *    offset 0: Start of a BSS section
 *    offset 4: Size of this BSS section. Must be multiply of 4
 */
	ldr	r3, =__zero_table_start__
	ldr	r4, =__zero_table_end__

.L_loop2:
	cmp	r3, r4
	bge	.L_loop2_done
	ldr	r1, [r3]
	ldr	r2, [r3, #4]
	movs	r0, 0

.L_loop2_0:
	subs	r2, #4
	itt	ge
	strge	r0, [r1, r2]
	bge	.L_loop2_0

	adds	r3, #8
	b	.L_loop2
.L_loop2_done:
#elif defined (__STARTUP_CLEAR_BSS)
/*  Single BSS section scheme.
 *
 *  The BSS section is specified by following symbols
 *    __bss_start__: start of the BSS section.
 *    __bss_end__: end of the BSS section.
 *
 *  Both addresses must be aligned to 4 bytes boundary.
 */
	ldr	r1, =__bss_start__
	ldr	r2, =__bss_end__

	movs	r0, 0
.L_loop3:
	cmp	r1, r2
	itt	lt
	strlt	r0, [r1], #4
	blt	.L_loop3
#endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */

#ifndef __NO_SYSTEM_INIT
	bl	SystemInit
#endif

#ifndef __START
#define __START _start
#endif
	bl	__START

#endif /* __GENERATE_DUMMY */
	.pool
	.size	Reset_Handler, . - Reset_Handler

	.align	1
	.thumb_func
	.weak	Default_Handler
	.type	Default_Handler, %function
Default_Handler:
	b	.
	.size	Default_Handler, . - Default_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
	.macro	def_irq_handler	handler_name
#ifdef __GENERATE_DUMMY
#else
	.weak	\handler_name
#endif /* __GENERATE_DUMMY */
	.set	\handler_name, Default_Handler
	.endm

	def_irq_handler	NMI_Handler
	def_irq_handler	HardFault_Handler
	def_irq_handler	MemManage_Handler
	def_irq_handler	BusFault_Handler
	def_irq_handler	UsageFault_Handler
	def_irq_handler	SVC_Handler
	def_irq_handler	DebugMon_Handler
	def_irq_handler	PendSV_Handler
	def_irq_handler	SysTick_Handler

/* IRQ Handlers */

	def_irq_handler	SW_IRQHandler
	def_irq_handler	TIM0_IRQHandler
	def_irq_handler	TIM1_IRQHandler
	def_irq_handler	TIM2_IRQHandler
	def_irq_handler	SYSTIME_IRQHandler
	def_irq_handler	WDG_IRQHandler
	def_irq_handler	DMAC_IRQHandler
	def_irq_handler	MCP_IRQHandler
	def_irq_handler	UART_IRQHandler
	def_irq_handler	I2C_IRQHandler
	def_irq_handler	I2CXPIC_IRQHandler
	def_irq_handler	ECC_IRQHandler
	def_irq_handler	XPICDEBUG_IRQHandler
	def_irq_handler	WDGXPIC_IRQHandler
	def_irq_handler	NFIFO_ARM_APP_IRQHandler
	def_irq_handler	IOLINK_IRQHandler
	def_irq_handler	SPI0_IRQHandler
	def_irq_handler	SPI1_IRQHandler
	def_irq_handler	SPI2_IRQHandler
	def_irq_handler	SPIXPIC_IRQHandler
	def_irq_handler	UARTAPP_IRQHandler
	def_irq_handler	UARTXPIC_IRQHandler
    def_irq_handler ASIC_IRQHandler
    def_irq_handler CLKSUP_IRQHandler
	def_irq_handler	SQI_IRQHandler
	def_irq_handler	HIFPIO_IRQHandler
	def_irq_handler	ETH_IRQHandler
	def_irq_handler	HASH_IRQHandler
	def_irq_handler	AES_IRQHandler
	def_irq_handler	MTGY_IRQHandler
	def_irq_handler	HIFRDY_IRQHandler
	def_irq_handler	IDPM_IRQHandler
    def_irq_handler HSC0_IRQHandler
    def_irq_handler HSC1_IRQHandler
    def_irq_handler HSC2_IRQHandler
    def_irq_handler HSC3_IRQHandler
    def_irq_handler HSC4_IRQHandler
    def_irq_handler HSC5_IRQHandler
    def_irq_handler HSC6_IRQHandler
    def_irq_handler HSC7_IRQHandler
    def_irq_handler HSC8TO15_IRQHandler
	def_irq_handler	ENDAT1_IRQHandler
	def_irq_handler	ENDAT2_IRQHandler
	def_irq_handler	BISS0_IRQHandler
	def_irq_handler	BISS1_IRQHandler
	def_irq_handler	MADC0_IRQHandler
	def_irq_handler	MADC1_IRQHandler
	def_irq_handler	MADC2_IRQHandler
	def_irq_handler	MADC3_IRQHandler
	def_irq_handler	MPWM_IRQHandler
	def_irq_handler	MENC0_IRQHandler
	def_irq_handler	MENC1_IRQHandler
	def_irq_handler	MENCCAP_IRQHandler
	def_irq_handler	MENCERR_IRQHandler
	def_irq_handler	GPIO0_IRQHandler
	def_irq_handler	GPIO1_IRQHandler
	def_irq_handler	GPIO2_IRQHandler
	def_irq_handler	GPIO3_IRQHandler
	def_irq_handler	GPIO4_IRQHandler
	def_irq_handler	GPIO5_IRQHandler
	def_irq_handler	GPIO6_IRQHandler
	def_irq_handler	GPIO7_IRQHandler
	def_irq_handler	GPIOTIM0_IRQHandler
	def_irq_handler	GPIOTIM1_IRQHandler
	def_irq_handler	GPIOTIM2_IRQHandler
	def_irq_handler	PIO0_IRQHandler
	def_irq_handler	PIO1_IRQHandler
	def_irq_handler	PIO2_IRQHandler
	def_irq_handler	PIO3_IRQHandler
	def_irq_handler	XCTRIGGER0_IRQHandler
	def_irq_handler	XCTRIGGER1_IRQHandler
	def_irq_handler	CAN0_IRQHandler
	def_irq_handler	CAN1_IRQHandler
	def_irq_handler	SQI0_IRQHandler
	def_irq_handler	SQI1_IRQHandler
    def_irq_handler CTI0_IRQHandler
    def_irq_handler CTI1_IRQHandler
	def_irq_handler	FPU_IRQHandler

	.end
