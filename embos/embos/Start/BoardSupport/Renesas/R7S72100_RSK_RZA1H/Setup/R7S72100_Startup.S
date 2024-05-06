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
File    : R7S72100_Startup.S
Purpose : Startup code for Renesas R7S72100
--------  END-OF-HEADER ----------------------------------------------
*/

/*********************************************************************
*
*       Defines, used for the processor status register
*
**********************************************************************
*/
        ARM_MODE_USER  = 0x10        /* Normal User Mode                             */
        ARM_MODE_FIQ   = 0x11        /* FIQ Fast Interrupts Mode                     */
        ARM_MODE_IRQ   = 0x12        /* IRQ Standard Interrupts Mode                 */
        ARM_MODE_SVC   = 0x13        /* Supervisor Interrupts Mode                   */
        ARM_MODE_ABORT = 0x17        /* Abort Processing memory Faults Mode          */
        ARM_MODE_UNDEF = 0x1B        /* Undefined Instructions Mode                  */
        ARM_MODE_SYS   = 0x1F        /* System Running in Priviledged Operating Mode */
        ARM_MODE_MASK  = 0x1F

        I_BIT          = 0x80        /* Disables IRQ when I bit is set               */
        F_BIT          = 0x40        /* Disables FIQ when F bit is set               */

/*********************************************************************
*
*       Vector table
*
**********************************************************************
*/
        .arm
        .section .vectors, "ax"
        .global  __vector
__vector:
        ldr     pc, =Reset_Handler   /* RESET                 vector */
        ldr     pc, =undef_handler   /* Undefined instruction vector */
        ldr     pc, =swi_handler     /* Software interrupt    vector */
        ldr     pc, =pabort_handler  /* Prefetch abort        vector */
        ldr     pc, =dabort_handler  /* Data abort            vector */
        nop                          /* Reserved              vector */
        ldr     pc,=IRQ_Handler      /* Interrupt             vector */
        ldr     pc,=fiq_handler      /* Fast interrupt        vector */

//==================================================================
// Reset Handler
//==================================================================

        .section .init, "ax"
        .arm
        .func
        .global Reset_Handler
        .type Reset_Handler, function
Reset_Handler:

//==================================================================
// Disable caches, MMU and branch prediction in case they were left enabled from an earlier run
// This does not need to be done from a cold reset
//==================================================================

        MRC     p15, 0, r0, c1, c0, 0       // Read CP15 System Control register
        BIC     r0, r0, #(0x1 << 12)        // Clear I bit 12 to disable I Cache
        BIC     r0, r0, #(0x1 <<  2)        // Clear C bit  2 to disable D Cache
        BIC     r0, r0, #0x1                // Clear M bit  0 to disable MMU
        BIC     r0, r0, #(0x1 << 11)        // Clear Z bit 11 to disable branch prediction
        BIC     r0, r0, #(0x1 << 13)        // Clear V bit 13 to disable high vector table
        BIC     r0, r0, #(0x1 << 28)        // Clear TRE bit to disable TEX remap
        MCR     p15, 0, r0, c1, c0, 0       // Write value back to CP15 System Control register

        MOV     r0, #0xF100                 // Disable L2 cache
        MOVT    r0, #0x3FFF
        MOV     r1, #0x00
        STR     r1, [r0, #0]

        LDR     r0,=__vector                // Load exception table base address to R0
        MCR     p15, 0, r0, c12, c0, 0      // Write address to CP15.VBAR register.

        /*
         * Setup a stack for each mode
         */
        msr   CPSR_c, #ARM_MODE_UNDEF | I_BIT | F_BIT   /* Undefined Instruction Mode */
        ldr   sp, =__stack_und_end__
        bic   sp,sp,#0x7              /* Make sure SP is 8 aligned */

        msr   CPSR_c, #ARM_MODE_ABORT | I_BIT | F_BIT   /* Abort Mode */
        ldr   sp, =__stack_abt_end__
        bic   sp,sp,#0x7              /* Make sure SP is 8 aligned */

        msr   CPSR_c, #ARM_MODE_FIQ   | I_BIT | F_BIT   /* FIQ Mode */
        ldr   sp, =__stack_fiq_end__
        bic   sp,sp,#0x7              /* Make sure SP is 8 aligned */

        msr   CPSR_c, #ARM_MODE_IRQ   | I_BIT | F_BIT   /* IRQ Mode */
        ldr   sp, =__stack_irq_end__
        bic   sp,sp,#0x7              /* Make sure SP is 8 aligned */

        msr   CPSR_c, #ARM_MODE_SVC   | I_BIT | F_BIT   /* Supervisor Mode */
        ldr   sp, =__stack_svc_end__
        bic   sp,sp,#0x7              /* Make sure SP is 8 aligned */

//==================================================================
// Enable VFP/NEON unit
//==================================================================
        //
        // Enable access to co-processor cp10 and cp11
        // for both kernel and user access
        //
        mrc     p15, 0, r0, c1, c0, 2
        orr     r0, r0, #(3 << 22)         // Kernel+user access to cp11
        orr     r0, r0, #(3 << 20)         // Kernel+user access to cp10
        mcr     p15, 0, r0, c1, c0, 2      // Set co-processor access bits

        //
        // Enable VFP co-processor by setting EN bit in FPEXC register
        //
        mov     r0, #0
        orr     r0, r0, #(1 << 30)         // EN (1=enable VFPLite and NEON coprocessor)
        mcr     p10, 7, r0, c8, c0, 0      // Set FPEXC register

//==================================================================
// Invalidate Data and Instruction TLBs and branch predictor
//==================================================================

        MOV     r0,#0
        MCR     p15, 0, r0, c8, c7, 0      // I-TLB and D-TLB invalidation
        MCR     p15, 0, r0, c7, c5, 6      // BPIALL - Invalidate entire branch predictor array

//==================================================================
// Cache Invalidation code for Cortex-A9
//==================================================================

        // Invalidate L1 Instruction Cache

        MRC     p15, 1, r0, c0, c0, 1      // Read Cache Level ID Register (CLIDR)
        TST     r0, #0x3                   // Harvard Cache?
        MOV     r0, #0                     // SBZ
        MCRNE   p15, 0, r0, c7, c5, 0      // ICIALLU - Invalidate instruction cache and flush branch target cache

        // Invalidate Data/Unified Caches

        MRC     p15, 1, r0, c0, c0, 1      // Read CLIDR
        ANDS    r3, r0, #0x07000000        // Extract coherency level
        MOV     r3, r3, LSR #23            // Total cache levels << 1
        BEQ     Finished                   // If 0, no need to clean

        MOV     r10, #0                    // R10 holds current cache level << 1
Loop1:  ADD     r2, r10, r10, LSR #1       // R2 holds cache "Set" position
        MOV     r1, r0, LSR r2             // Bottom 3 bits are the Cache-type for this level
        AND     r1, r1, #7                 // Isolate those lower 3 bits
        CMP     r1, #2
        BLT     Skip                       // No cache or only instruction cache at this level

        MCR     p15, 2, r10, c0, c0, 0     // Write the Cache Size selection register
        ISB                                // ISB to sync the change to the CacheSizeID reg
        MRC     p15, 1, r1, c0, c0, 0      // Reads current Cache Size ID register
        AND     r2, r1, #7                 // Extract the line length field
        ADD     r2, r2, #4                 // Add 4 for the line length offset (log2 16 bytes)
        LDR     r4, =0x3FF
        ANDS    r4, r4, r1, LSR #3         // R4 is the max number on the way size (right aligned)
        CLZ     r5, r4                     // R5 is the bit position of the way size increment
        LDR     r7, =0x7FFF
        ANDS    r7, r7, r1, LSR #13        // R7 is the max number of the index size (right aligned)

Loop2:  MOV     r9, r4                     // R9 working copy of the max way size (right aligned)

Loop3:  ORR     r11, r10, r9, LSL r5       // Factor in the Way number and cache number into R11
        ORR     r11, r11, r7, LSL r2       // Factor in the Set number
        MCR     p15, 0, r11, c7, c6, 2     // Invalidate by Set/Way
        SUBS    r9, r9, #1                 // Decrement the Way number
        BGE     Loop3
        SUBS    r7, r7, #1                 // Decrement the Set number
        BGE     Loop2
Skip:   ADD     r10, r10, #2               // increment the cache number
        CMP     r3, r10
        BGT     Loop1

Finished:

InitTTB:
//===================================================================
// Cortex-A9 MMU Configuration
// Set translation table base
//===================================================================

        // Cortex-A9 supports two translation tables
        // Configure translation table base (TTB) control register cp15,c2
        // to a value of all zeros, indicates we are using TTB register 0.

        MOV     r0,#0x0
        MCR     p15, 0, r0, c2, c0, 2

// We don't initialze the TTB here, this is done in __low_level_init() in RTOSInit-C-module

        LDR     r12,=crt0                  // Save this in register for possible long jump
        BX      r12                        // Branch to __main in system mode

        .endfunc

//==================================================================
// Enable caches and branch prediction
// This code must be run from a privileged mode
//==================================================================


/*********************************************************************
*
*       Standard C (crt0) initialization function
*
**********************************************************************
*/

crt0:
        /*
         * Call __low_level_init to initiliaze hardware
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
        /*
         *  Prepare and call main()
         *  main() is called in SVC mode with interrupts disabled
         */
        mov   r0, #0                         /* No arguments are passed to main */
        mov   r1, #0
        ldr   r2, =main
        mov   lr, pc
        bx    r2
_exit:  b     _exit                          /* We should never arrive here, just for sureness. */

/*********************************************************************
*
*       __low_level_init
*
**********************************************************************
*/
__low_level_init:
        bx lr
        .weak __low_level_init


/*********************************************************************
*
*       Default exception handlers
*
*  These are declared weak symbols so the can be redefined in user code.
*
**********************************************************************
*/
        .section .init, "ax"
        .code 32
        .balign 4

        .weak undef_handler
        .type undef_handler, function
undef_handler:
        b       undef_handler

        .weak swi_handler
        .type swi_handler, function
swi_handler:
        b       swi_handler

        .weak pabort_handler
        .type pabort_handler, function
pabort_handler:
        b       pabort_handler

        .weak dabort_handler
        .type dabort_handler, function
dabort_handler:
        b       dabort_handler

        .weak irq_handler
        .type irq_handler, function
irq_handler:
        b       irq_handler

        .weak fiq_handler
        .type fiq_handler, function
fiq_handler:
        b       fiq_handler

//==================================================================
        .end
//==================================================================



