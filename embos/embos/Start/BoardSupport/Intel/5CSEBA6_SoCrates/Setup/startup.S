/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2022 SEGGER Microcontroller GmbH                  *
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
*       OS version: V5.18.0.0                                        *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------

File   : startup.S
Purpose: Startup code for Cotex A9 used with GNU compiler
-------- END-OF-HEADER -----------------------------------------------
*/


/*********************************************************************
*
*  Defines, used for the processor status register
*/

        ARM_MODE_USER  = 0x10       /* Normal User Mode                             */
        ARM_MODE_FIQ   = 0x11       /* FIQ Fast Interrupts Mode                     */
        ARM_MODE_IRQ   = 0x12       /* IRQ Standard Interrupts Mode                 */
        ARM_MODE_SVC   = 0x13       /* Supervisor Interrupts Mode                   */
        ARM_MODE_ABORT = 0x17       /* Abort Processing memory Faults Mode          */
        ARM_MODE_UNDEF = 0x1B       /* Undefined Instructions Mode                  */
        ARM_MODE_SYS   = 0x1F       /* System Running in Priviledged Operating Mode */
        ARM_MODE_MASK  = 0x1F

        I_BIT          = 0x80       /* Disables IRQ when I bit is set               */
        F_BIT          = 0x40       /* Disables FIQ when F bit is set               */


/************************************************************************************
*
*       Vector table for ARM core.
*/
        .text
        .global  __vector
        .extern  Reset_Handler

        .arm
        .section .vectors, "ax"

__vector:
        ldr     pc,Reset_Addr   /* RESET                 vector */
        ldr     pc,Undef_Addr   /* Undefined instruction vector */
        ldr     pc,SWI_Addr     /* Software interrupt    vector */
        ldr     pc,PAbt_Addr    /* Prefetch abort        vector */
        ldr     pc,DAbt_Addr    /* Data abort            vector */
        nop                     /* Reserved              vector */
        ldr     pc,IRQ_Addr     /* Interrupt             vector */
        ldr     pc,FIQ_Addr     /* Fast interrupt        vector */

Reset_Addr:     .word   Reset_Handler
Undef_Addr:     .word   Undef_Handler
SWI_Addr:       .word   SWI_Handler
PAbt_Addr:      .word   PAbt_Handler
DAbt_Addr:      .word   DAbt_Handler
IRQ_Addr:       .word   IRQ_Handler
FIQ_Addr:       .word   FIQ_Handler


/* ; The following dummy handlers do not do anything useful in this example.
*  ; They are set up here for completeness.
*  
*  ; IRQ_Handler has to be declared and exported as weak.
*  ; It will be replaced by the IRQ_Handler from the embOS library
*  
*  ; FIQ_Handler is declared and exported as weak.
*  ; This allows replacement by the application
*/
        .weak   Undef_Handler
        .weak   SWI_Handler
        .weak   PAbt_Handler
        .weak   DAbt_Handler
        .weak   IRQ_Handler
        .weak   FIQ_Handler

        .extern Undef_Handler
        .extern SWI_Handler
        .extern PAbt_Handler
        .extern DAbt_Handler
        .extern IRQ_Handler
        .extern FIQ_Handler

Undef_Handler:  b       Undef_Handler
SWI_Handler:    b       SWI_Handler
PAbt_Handler:   b       PAbt_Handler
DAbt_Handler:   b       DAbt_Handler
IRQ_Handler:    b       IRQ_Handler
FIQ_Handler:    b       FIQ_Handler
__vector_end:

/********************************************************************/


/**********************************************************************
*
*       Reset_Handler
*
*       Execution starts here.
*       After a reset, the mode is ARM, Supervisor, interrupts disabled.
*/
        .global  Reset_Handler
        .global  end
        .arm
        .section .text, "ax"  
        .func

Reset_Handler:
        MRC     p15, 0, r0, c1, c0, 0       /* ; Read CP15 System Control register */
        BIC     r0, r0, #(0x1 << 12)        /* ; Disable I Cache */
        BIC     r0, r0, #(0x1 <<  2)        /* ; Disable D Cache */
        BIC     r0, r0, #0x1                /* ; Disable MMU     */
        BIC     r0, r0, #(0x1 << 11)        /* ; Disable branch predictor      */
        MCR     p15, 0, r0, c1, c0, 0       /* ; Setup mode */

        /*
         * Setup a stack for IRQ and SVC mode, stacks have to be aligned to 8 byte boundary
         */
        MSR     CPSR_c, #ARM_MODE_IRQ | I_BIT | F_BIT   /* IRQ Mode with interrupts disabled */
        LDR     R0, =__stack_irq_end__                  /* Symbol from linker script */
        BIC     R0,R0,#0x7                              /* Ensure SP is aligned on 8 byte boundary */
        MOV     SP, R0

        /*
         * Other stacks may be initialized here if needed.
         */
        MSR     CPSR_c, #ARM_MODE_FIQ   | I_BIT | F_BIT /* FIQ Mode with interrupts disabled */
        LDR     R0, =__stack_fiq_end__                  /* Symbol from linker script */
        BIC     R0,R0,#0x7                              /* Ensure SP is aligned on 8 byte boundary */
        MOV     SP, R0

        /*
         * SVC mode should be the last mode set, main() should be entered in SVC mode
         */
        MSR     CPSR_c, #ARM_MODE_SVC   | I_BIT | F_BIT /* Supervisor Mode with interrupts disabled */
        LDR     R0, =__stack_svc_end__                  /* Symbol from linker script */
        BIC     R0,R0,#0x7                              /* Ensure SP is aligned on 8 byte boundary */
        MOV     SP, R0

        /*
         * Invalidate Data and Instruction TLBs and branch predictor
         */
        MOV     r0,#0
        MCR     p15, 0, r0, c8, c7, 0      /* I-TLB and D-TLB invalidation */
        MCR     p15, 0, r0, c7, c5, 6      /* BPIALL - Invalidate entire branch predictor array */


        /*
         * Set Vector Base Address Register (VBAR) to point to this application's vector table
         */
        LDR     r0, =__vector
        MCR     p15, 0, r0, c12, c0, 0

        /*
         * Invalidate cache (Code for Cortex A9)
         */

        /* Invalidate L1 Instruction Cache */

        MRC     p15, 1, r0, c0, c0, 1      /* ; Read Cache Level ID Register (CLIDR) */
        TST     r0, #0x3                   /* ; Harvard Cache?                       */
        MOV     r0, #0                     /* ; SBZ                                  */
        MCRNE   p15, 0, r0, c7, c5, 0      /* ; ICIALLU - Invalidate instruction cache and flush branch target cache */
        
        /* Invalidate Data/Unified Caches */
        
        MRC     p15, 1, r0, c0, c0, 1      /* ; Read CLIDR */
        ANDS    r3, r0, #0x07000000        /* ; Extract coherency level */
        MOV     r3, r3, LSR #23            /* ; Total cache levels << 1 */
        BEQ     Finished                   /* ; If 0, no need to clean */

        MOV     r10, #0                    /* ; R10 holds current cache level << 1 */
Loop1:  ADD     r2, r10, r10, LSR #1       /* ; R2 holds cache "Set" position  */
        MOV     r1, r0, LSR r2             /* ; Bottom 3 bits are the Cache-type for this level */
        AND     r1, r1, #7                 /* ; Isolate those lower 3 bits */
        CMP     r1, #2
        BLT     Skip                       /* ; No cache or only instruction cache at this level */

        MCR     p15, 2, r10, c0, c0, 0     /* ; Write the Cache Size selection register */
        ISB                                /* ; ISB to sync the change to the CacheSizeID reg */
        MRC     p15, 1, r1, c0, c0, 0      /* ; Reads current Cache Size ID register */
        AND     r2, r1, #7                 /* ; Extract the line length field */
        ADD     r2, r2, #4                 /* ; Add 4 for the line length offset (log2 16 bytes) */
        LDR     r4, =0x3FF
        ANDS    r4, r4, r1, LSR #3         /* ; R4 is the max number on the way size (right aligned) */
        CLZ     r5, r4                     /* ; R5 is the bit position of the way size increment */
        LDR     r7, =0x7FFF
        ANDS    r7, r7, r1, LSR #13        /* ; R7 is the max number of the index size (right aligned) */

Loop2:  MOV     r9, r4                     /* ; R9 working copy of the max way size (right aligned) */

Loop3:  ORR     r11, r10, r9, LSL r5       /* ; Factor in the Way number and cache number into R11 */
        ORR     r11, r11, r7, LSL r2       /* ; Factor in the Set number */
        MCR     p15, 0, r11, c7, c6, 2     /* ; Invalidate by Set/Way */
        SUBS    r9, r9, #1                 /* ; Decrement the Way number */
        BGE     Loop3
        SUBS    r7, r7, #1                 /* ; Decrement the Set number */
        BGE     Loop2
Skip:   ADD     r10, r10, #2               /* ; increment the cache number */
        CMP     r3, r10
        BGT     Loop1

Finished:

        /*
         *  Cortex-A9 MMU Configuration
         *  Prepare and set translation table base
         */
   
        /* Cortex-A9 supports two translation tables
         *  Configure translation table base (TTB) control register cp15,c2
         * ; to a value of all zeros, indicates we are using TTB register 0.
         */

        MOV     r0,#0x0
        MCR     p15, 0, r0, c2, c0, 2

        /* We don't initialize the MMU table here, this is done in __low_level_init() in the RTOSInit C-File */

        /*
         * Initialize the Floating point unit
         */

    #if (TARGET_FEATURE_NEON) || (TARGET_FPU_VFP)
        /*
         * Enable access to NEON/VFP by enabling access to Coprocessors 10 and 11.
         * Enables Full Access i.e. in both privileged and non privileged modes
         */

        MRC     p15, 0, r0, c1, c0, 2      /* Read Coprocessor Access Control Register (CPACR) */
        ORR     r0, r0, #(0xF << 20)       /* Enable access to CP 10 & 11 */
        MCR     p15, 0, r0, c1, c0, 2      /* Write Coprocessor Access Control Register (CPACR) */
        ISB

        /*
         * Switch on the VFP and NEON hardware
         */

        MOV     r0, #0x40000000
        VMSR    FPEXC, r0                  /* Write FPEXC register, EN bit set */
    #endif

/****** Now call the standard startup */

        LDR     r12,=crt0                  /* Save this in register for possible long jump */
        MOV     lr, pc
        BX      r12                        /* Branch to crt0 in SVC mode */

end:    B       end
        
        .endfunc                           /* Reset_Handler */


/*********************************************************************
*
*       crt0()
*       Standard C initialization function
*       Calls Low_level_init, copies data from ROM to RAM, clears BSS,
*       finally calls main().
*/
        .extern  __low_level_init
        .extern  main

        .weak    _exit
        .extern  _exit

        .func
crt0:
        /*
         * Call __low_level_init to initialize hardware
         * before calling c-standard startup
         */

        ldr     r0,=__low_level_init
        mov     lr, pc
        bx      r0

        /*
         * Relocate .data section
         * (Copy from ROM to RAM)
         */

        ldr   r1, =__data_load__
        ldr   r2, =__data_start__
        ldr   r3, =__data_end__
LoopRel:
        cmp   r2, r3
        ldrlo r0, [r1], #4
        strlo r0, [r2], #4
        blo   LoopRel

        /*
         * Clear .bss section
         */
        ldr   r1, =__bss_start__
        ldr   r2, =__bss_end__
        ldr   r3, =0
bss_clear_loop:
        cmp   r1, r2
        strne r3, [r1], #+4
        bne   bss_clear_loop


        /* 4. Run static constructors  */
        bl __libc_init_array

        /*
         *  Prepare and call main(). Main () is called in SVC mode with interrupts disabled
         */

        mov   r0, #0        /* No arguments are passed to main */
        mov   r1, #0
        ldr   r2, =main
        mov   lr, pc
        bx    r2

_exit:  b     _exit         /* We should never arrive here, just for sureness. */

        .endfunc            /* crt0 */

/********************************************************************/

        .end

/****** End Of File *************************************************/
