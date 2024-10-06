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

-------------------------- END-OF-HEADER -----------------------------
File    : RTOSInit_5CSEBA6.c
Purpose : Initializes and handles the hardware for embOS

Literature:
  [1]  Generic Interrupt Controller (GIC-390) Errata Notice
       (\\fileserver\Techinfo\Company\ARM\GIC_GenericInterruptController\GIC_390_Errata_Notice_v6.pdf)
*/

#include "RTOS.h"

#if (defined(__ICCARM__))
  #include <intrinsics.h>
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/*********************************************************************
*
*       System tick settings
*/
#define OS_TIMER_FREQ  (800000000u / 4)
#define OS_TICK_FREQ   (1000u)
#define OS_INT_FREQ    (OS_TICK_FREQ)

/*********************************************************************
*
*       embOSView settings
*/
#ifndef   OS_VIEW_IFSELECT
  #define OS_VIEW_IFSELECT  OS_VIEW_IF_JLINK
#endif

#if (OS_VIEW_IFSELECT == OS_VIEW_IF_JLINK)
  #include "JLINKDCC.h"
#elif (OS_VIEW_IFSELECT == OS_VIEW_IF_UART)
  #include "BSP_UART.h"
  #define OS_UART      (0u)
  #define OS_BAUDRATE  (38400u)
#endif

/*********************************************************************
*
*       Device specific SFRs
*/
#define SCU_BASE_ADDR   (0xFFFEC000u)

//
// Private timer
//
#define PTIMER_BASE_ADDR   (SCU_BASE_ADDR + 0x0600u)
#define PTIMER_LOAD        (*(volatile OS_U32*)(PTIMER_BASE_ADDR + 0x00u))
#define PTIMER_COUNT       (*(volatile OS_U32*)(PTIMER_BASE_ADDR + 0x04u))
#define PTIMER_CONTROL     (*(volatile OS_U32*)(PTIMER_BASE_ADDR + 0x08u))
#define PTIMER_STATUS      (*(volatile OS_U32*)(PTIMER_BASE_ADDR + 0x0Cu))

#define PTIMER_INT_ID      (29)

//
// Watchdog
//
#define WD_LOAD            (*(volatile OS_U32*)(PTIMER_BASE_ADDR + 0x20u))
#define WD_COUNT           (*(volatile OS_U32*)(PTIMER_BASE_ADDR + 0x24u))
#define WD_CONTROL         (*(volatile OS_U32*)(PTIMER_BASE_ADDR + 0x28u))
#define WD_INT_STATUS      (*(volatile OS_U32*)(PTIMER_BASE_ADDR + 0x2Cu))
#define WD_RESET_STATUS    (*(volatile OS_U32*)(PTIMER_BASE_ADDR + 0x30u))
#define WD_DISABLE         (*(volatile OS_U32*)(PTIMER_BASE_ADDR + 0x34u))

//
// Generic interrupt controller
//
#define GICD_BASE_ADDR     (0xFFFED000u)  // Distributor interface
#define GICC_BASE_ADDR     (0xFFFEC100u)  // CPU interface
#define NUM_INT_SOURCES    (256u)

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/
extern unsigned int __vector;     // Start of vector table, defined in startup file

/*********************************************************************
*
*       MMU and cache configuration
*
*  Notes
*    The translation table has to be aligned on 16384 bytes boundary
*    The translation table has to be located in un-initialized memory,
*    because it may be filled from startup, before segments and variables
*    are initialized.
*/
#if   (defined(__ICCARM__))  // IAR
  #pragma data_alignment=16384
  static __no_init OS_U32 _TranslationTable[0x1000];
#elif (defined(__CC_ARM))    // Keil
  #pragma arm section zidata = "NO_INIT"  // Has to be declared as separate section UNINIT in the scatter file
  __align(16384) static OS_U32 _TranslationTable[0x1000];
  #pragma arm section zidata              // back to default (.bss section)
#elif (defined(__GNUC__))    // GCC
  static OS_U32 _TranslationTable[0x1000] __attribute__ ((aligned (16384), section (".non_init")));
#endif

/*********************************************************************
*
*       Interrupt vector table
*
*  Notes
*    The translation table has to be located in un-initialized memory,
*    because it may be filled from startup, before segments and variables
*    are initialized.
*/
#if   (defined(__ICCARM__))  // IAR
  #pragma data_alignment=4
  static __no_init OS_ISR_HANDLER* _apIsrHandler[NUM_INT_SOURCES];
#elif (defined(__CC_ARM))    // Keil
  #pragma arm section zidata = "NO_INIT"  // Has to be declared as separate section UNINIT in the scatter file
  __align(4) static OS_ISR_HANDLER* _apIsrHandler[NUM_INT_SOURCES];
  #pragma arm section zidata              // back to default (.bss section)
#elif (defined(__GNUC__))    // GCC
  static OS_ISR_HANDLER* _apIsrHandler[NUM_INT_SOURCES] __attribute__ ((aligned (4), section (".non_init")));
#endif

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

#if (OS_VIEW_IFSELECT == OS_VIEW_IF_JLINK)
/*********************************************************************
*
*       _DCC_OnRx()
*/
static void _DCC_OnRx(unsigned Channel, OS_U8 Data) {
  OS_USE_PARA(Channel);  // Not supported by this version, avoid compiler warning
  OS_COM_OnRx(Data);
}

/*********************************************************************
*
*       _DCC_OnTx()
*/
static void _DCC_OnTx(unsigned Channel) {
  OS_USE_PARA(Channel);  // Not supported by this version, suppress warning
  OS_COM_OnTx();
}

#elif (OS_VIEW_IFSELECT == OS_VIEW_IF_UART)
/*********************************************************************
*
*       _OS_OnRX()
*
*  Function description
*    Callback wrapper function for BSP UART module.
*/
static void _OS_OnRX(unsigned int Unit, unsigned char c) {
  OS_USE_PARA(Unit);
  OS_COM_OnRx(c);
}

/*********************************************************************
*
*       _OS_OnTX()
*
*  Function description
*    Callback wrapper function for BSP UART module.
*/
static int _OS_OnTX(unsigned int Unit) {
  OS_USE_PARA(Unit);
  return (int)OS_COM_OnTx();
}
#endif

/*********************************************************************
*
*       _OS_ISR_Undefined()
*
*  Function description
*    Is called when a non-installed interrupt was detected.
*    As interrupt pending condition of peripherals has to be reset,
*    program will not continue when interrupt is ignored.
*/
static void _OS_ISR_Undefined(void) {
  volatile int Dummy;

  Dummy = 1;
  while (Dummy > 0) {
    ;  // You may set a breakpoint here to detect non-installed ISR-handler
  }
}

/*********************************************************************
*
*       _SpuriousIrqHandler()
*
*  Function description
*    default spurious IRQ handler
*
*  Notes
*    A spurious interrupt is called under one of the following
*    circumstances:
*    1. An external interrupt source is programmed in level-sensitive mode
*       and an active level occurs for only a short time.
*    2. An internal interrupt source is programmed in level sensitive
*       and the output signal of the corresponding embedded peripheral
*       is activated for a short time. (As in the case for the Watchdog.)
*    3. An interrupt occurs just a few cycles before the software begins
*       to mask it, thus resulting in a pulse on the interrupt source.
*
*    The spurious handler is called to detect these errors.
*/
static void _SpuriousIrqHandler(void) {
#if (OS_DEBUG != 0)
  static OS_U32 _SpuriousIrqCnt;

  _SpuriousIrqCnt++;
#endif
}

/*********************************************************************
*
*       _InitGIC()
*
*  Function description
*       Initialize interrupt controller by setting default vectors
*       and clearing all interrupts
*/
static void _InitGIC(void) {
  OS_U32 i;

  //
  // Set GIC base address
  //
  OS_GIC_SetCPUInterfaceBaseAddr(GICC_BASE_ADDR);
  OS_GIC_SetDistributionBaseAddr(GICD_BASE_ADDR);
  //
  // Configure all interrupts
  //
  for (i = 0u; i < NUM_INT_SOURCES; i++) {
    _apIsrHandler[i] = _OS_ISR_Undefined;  // Set default interrupt handler
    OS_GIC_SetIntSecure(i);                // Set to secure
    OS_GIC_DisableInt(i);                  // Disable interrupts
    OS_GIC_ClrPending(i);                  // Clear pending flags
    //
    // Initialize all Interrupt pointer target registers to address
    // the first core. The first 8 registers are fixed.
    //
    if (i > 8u) {
      OS_GIC_SetTargetProc(i, 0x01u);
    }
  }
  //
  // Configure priority mask to lowest value (highest number) - enables all priorities
  //
  OS_GIC_SetPrioThreshold(0xFFu);
  //
  // This register defines the point at which the priority value fields split
  // into two parts, the group priority field and the sub-priority field.
  //
  OS_GIC_SetPrioGroup(0x00u);
  //
  // Enable the GIC
  //
  OS_GIC_Enable();
}

/*********************************************************************
*
*       _OS_GetHWTimerCycles()
*
*  Function description
*    Returns the current hardware timer count value.
*
*  Return value
*    Current timer count value.
*/
static unsigned int _OS_GetHWTimerCycles(void) {
  return PTIMER_COUNT;
}

/*********************************************************************
*
*       _OS_GetHWTimer_IntPending()
*
*  Function description
*    Returns if the hardware timer interrupt pending flag is set.
*
*  Return value
*    == 0: Interrupt pending flag not set.
*    != 0: Interrupt pending flag set.
*/
static unsigned int _OS_GetHWTimer_IntPending(void) {
  return (OS_ARM_IsPending(PTIMER_INT_ID));
}

/*********************************************************************
*
*       _OS_ISR_Tick()
*
*  Function description
*    This is the hardware timer exception handler.
*/
static void _OS_ISR_Tick(void) {
  PTIMER_STATUS = (1u << 0);  // Clear timer interrupt pending flag
  OS_TICK_Handle();
#if ((defined(__ICCARM__) && (OS_DEBUG != 0)) || (OS_VIEW_IFSELECT == OS_VIEW_IF_JLINK))
  JLINKDCC_Process();
#endif
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       OS_InitHW()
*
*  Function description
*    Initialize the hardware required for embOS to run.
*/
void OS_InitHW(void) {
  volatile OS_U32 Dummy;
           OS_U32 TickPrio;

  OS_INT_IncDI();
  //
  // Ensure caches are synchronized
  //
  OS_ARM_CACHE_Sync();
  //
  // Set vector table base address
  //
  OS_ARM_SetVBAR((OS_U32)&__vector);
  //
  // Initialize interrupt controller
  //
  _InitGIC();
  //
  // Initialize timer for embOS
  //
  OS_ARM_InstallISRHandler(PTIMER_INT_ID, _OS_ISR_Tick);
  OS_ARM_ISRSetPrio(PTIMER_INT_ID, 0xFFu);              // Set to lowest level, ALL BITS set
  TickPrio  = OS_ARM_ISRSetPrio(PTIMER_INT_ID, 0xFFu);  // Read back priority to examine relevant preemption-level-bits
  TickPrio -= 1;                                        // Set to low preemption level, 1 level higher than lowest
  OS_ARM_ISRSetPrio(PTIMER_INT_ID, TickPrio);
  OS_ARM_ClearPendingFlag(PTIMER_INT_ID);
  OS_ARM_EnableISR(PTIMER_INT_ID);
  PTIMER_CONTROL = 0x0uL;                                 // Stop Timer, set Prescaler to 1
  PTIMER_LOAD    = ((OS_TIMER_FREQ / OS_TICK_FREQ) - 1);  // Set timer interval
  PTIMER_CONTROL = (1u << 0)                              // Enable Timer
                 | (1u << 1)                              // Set auto reload mode
                 | (1u << 2)                              // Enable timer interrupt
                 ;
  //
  // Inform embOS about the timer settings
  //
  {
    OS_SYSTIMER_CONFIG SysTimerConfig = {OS_TIMER_FREQ, OS_INT_FREQ, OS_TIMER_DOWNCOUNTING, _OS_GetHWTimerCycles, _OS_GetHWTimer_IntPending};
    OS_TIME_ConfigSysTimer(&SysTimerConfig);
  }
#if (OS_SUPPORT_SAVE_RESTORE_HOOK != 0)
  //
  // Set a default context extension in order to preserve NEON registers for each task.
  // The NEON unit uses 32 VFP double-precision registers which need to be preserved.
  // Please consider that by setting a default VFP context extension, each task that
  // does not use the VFP or NEON unit will still preserve their registers unnecessarily.
  // Therefore, it is recommended to set the context extension individually for each task.
  // Furthermore, if the VFP or NEON units are used from within ISRs, the according functions
  // OS_VFPD16_SaveRegs()/OS_VFPD16_RestoreRegs() or OS_VFPD32_SaveRegs()/OS_VFPD32_RestoreRegs()
  // need to be used to preserve the registers.
  //
  OS_TASK_SetDefaultContextExtension(&OS_VFPD32_ContextExtension);
#endif
  //
  // Initialize communication for embOSView
  //
#if (OS_VIEW_IFSELECT == OS_VIEW_IF_JLINK)
  JLINKDCC_SetpfOnRx(1, _DCC_OnRx);
  JLINKDCC_SetpfOnTx(1, _DCC_OnTx);
#elif (OS_VIEW_IFSELECT == OS_VIEW_IF_UART)
  BSP_UART_Init(OS_UART, OS_BAUDRATE, BSP_UART_DATA_BITS_8, BSP_UART_PARITY_NONE, BSP_UART_STOP_BITS_1);
  BSP_UART_SetReadCallback(OS_UART, _OS_OnRX);
  BSP_UART_SetWriteCallback(OS_UART, _OS_OnTX);
#elif (OS_VIEW_IFSELECT == OS_VIEW_IF_ETHERNET)
  UDP_Process_Init();
#endif
  OS_INT_DecRI();
}

/*********************************************************************
*
*       OS_Idle()
*
*  Function description
*    This code is executed whenever no task, software timer, or
*    interrupt is ready for execution.
*
*  Additional information
*    The idle loop does not have a stack of its own, therefore no
*    functionality should be implemented that relies on the stack
*    to be preserved.
*/
void OS_Idle(void) {  // Idle loop: No task is ready to execute
  while (1) {         // Nothing to do ... wait for interrupt
    #if ((OS_VIEW_IFSELECT != OS_VIEW_IF_JLINK) && (OS_DEBUG == 0))
                      // Switch CPU into sleep mode
    #endif
  }
}

/*********************************************************************
*
*       Optional communication with embOSView
*
**********************************************************************
*/

/*********************************************************************
*
*       OS_COM_Send1()
*
*  Function description
*    Sends one character.
*/
void OS_COM_Send1(OS_U8 c) {
#if (OS_VIEW_IFSELECT == OS_VIEW_IF_JLINK)
  JLINKDCC_SendCharOnChannelNB(1, c);
#elif (OS_VIEW_IFSELECT == OS_VIEW_IF_UART)
  BSP_UART_Write1(OS_UART, c);
#elif (OS_VIEW_IFSELECT == OS_VIEW_IF_ETHERNET)
  UDP_Process_Send1(c);
#elif (OS_VIEW_IFSELECT == OS_VIEW_DISABLED)
  OS_USE_PARA(c);          // Avoid compiler warning
  OS_COM_ClearTxActive();  // Let embOS know that Tx is not busy
#endif
}

/*********************************************************************
*
*       embOS interrupt handler and ISR specific functions
*
**********************************************************************
*/

/*********************************************************************
*
*       OS_irq_handler()
*
*  Function description
*    Detect reason for IRQ and call corresponding service routine.
*    OS_irq_handler is called from the IRQ_handler function
*    found in RTOS.s.
*    IRQ_Handler has to be inserted in the vector table in the
*    startup code.
*/
OS_INTERWORK void OS_irq_handler(void) {
  OS_ISR_HANDLER* pISR;
  OS_U32          int_id;

  int_id = OS_GIC_AckInt();  // Acknowledge the interrupt and read the interrupt ID
  if ((int_id == 0x3FEu) || (int_id == 0x3FFu)) {  // Check for spurious interrupts
     _SpuriousIrqHandler();
  } else {
    if (int_id < NUM_INT_SOURCES) {
      pISR = _apIsrHandler[int_id];      // Get interrupt vector handler
      OS_INT_Enter();
      pISR();                                      // Call interrupt service routine
      OS_INT_Leave();
    }
    OS_GIC_EOI(int_id);                  // End interrupt
  }
}

/*********************************************************************
*
*       OS_ARM_InstallISRHandler()
*
*  Function description
*    Installs an interrupt handler
*
*  Parameters
*    ISRIndex:    Index of the interrupt source, usually the interrupt vector number
*    pISRHandler: Address of the interrupt handler function
*
*  Return value
*    The address of the interrupt handler that was previously installed with the addressed interrupt source
*/
OS_ISR_HANDLER* OS_ARM_InstallISRHandler(int ISRIndex, OS_ISR_HANDLER* pISRHandler) {
  OS_ISR_HANDLER* pOldHandler;

#if (OS_DEBUG != 0)
  if ((unsigned int)ISRIndex >= NUM_INT_SOURCES) {
    OS_Error(OS_ERR_ISR_INDEX);
  }
#endif
  OS_INT_Disable();
  pOldHandler             = _apIsrHandler[ISRIndex];
  _apIsrHandler[ISRIndex] = pISRHandler;
  OS_INT_EnableConditional();
  return pOldHandler;
}

/*********************************************************************
*
*       OS_ARM_EnableISR()
*
*  Function description
*    Enables an interrupt
*
*  Parameters
*    ISRIndex: Index of the interrupt source which should be enabled
*/
void OS_ARM_EnableISR(int ISRIndex) {
#if (OS_DEBUG != 0)
  if ((unsigned int)ISRIndex >= NUM_INT_SOURCES) {
    OS_Error(OS_ERR_ISR_INDEX);
  }
#endif
  OS_INT_Disable();
  //
  // Program the distributor to trigger the selected CPU core
  //
  OS_GIC_SetTargetProc(ISRIndex, 0x01);
  OS_GIC_EnableInt(ISRIndex);
  OS_INT_EnableConditional();
}

/*********************************************************************
*
*       OS_ARM_DisableISR()
*
*  Function description
*    Disables an interrupt
*
*  Parameters
*    ISRIndex: Index of the interrupt source which should be disabled
*/
void OS_ARM_DisableISR(int ISRIndex) {
#if (OS_DEBUG != 0)
  if ((unsigned int)ISRIndex >= NUM_INT_SOURCES) {
    OS_Error(OS_ERR_ISR_INDEX);
  }
#endif
  OS_INT_Disable();
  OS_GIC_DisableInt(ISRIndex);
  OS_INT_EnableConditional();
}

/*********************************************************************
*
*       OS_ARM_ISRSetPrio()
*
*  Function description
*    Sets the priority of an interrupt
*
*  Parameters
*    ISRIndex: Index of the interrupt source for which to configure the priority
*    Prio    : The priority which should be set for the specific interrupt
*/
int OS_ARM_ISRSetPrio(int ISRIndex, int Prio) {
  OS_U32 OldPrio;

#if (OS_DEBUG != 0)
  if ((unsigned int)ISRIndex >= NUM_INT_SOURCES) {
    OS_Error(OS_ERR_ISR_INDEX);
  }
#endif
  OS_INT_Disable();
  OldPrio = OS_GIC_SetPrio(ISRIndex, Prio);
  OS_INT_EnableConditional();
  return (int)OldPrio;
}

/*********************************************************************
*
*       OS_ARM_ClearPendingFlag()
*
*  Function description
*    Clears an interrupt pending flag
*
*  Parameters
*    ISRIndex: Index of the interrupt source for which to clear the flag
*/
void OS_ARM_ClearPendingFlag(int ISRIndex) {
#if (OS_DEBUG != 0)
  if ((unsigned int)ISRIndex >= NUM_INT_SOURCES) {
    OS_Error(OS_ERR_ISR_INDEX);
  }
#endif
  OS_INT_Disable();
  OS_GIC_ClrPending(ISRIndex);
  OS_INT_EnableConditional();
}

/*********************************************************************
*
*       OS_ARM_IsPending()
*
*  Function description
*    Checks if an interrupt is pending
*
*  Parameters
*    ISRIndex: Index of the interrupt source that shall be checked
*
*  Return value
*    == 0; Interrupt is not pending
*    != 0: Interrupt is pending
*/
unsigned int OS_ARM_IsPending(int ISRIndex) {
  OS_U32 Result;

#if (OS_DEBUG != 0)
  if ((unsigned int)ISRIndex >= NUM_INT_SOURCES) {
    OS_Error(OS_ERR_ISR_INDEX);
  }
#endif
  OS_INT_Disable();
  Result = OS_GIC_IsPending(ISRIndex);
  OS_INT_EnableConditional();
  return Result;
}

/*********************************************************************
*
*       __low_level_init()
*
*  Function description
*    Initialize memory controller, clock generation and PLL.
*
*    Has to be modified if another CPU clock frequency should be
*    used. This function is called during startup and has to
*    return 1 to perform segment initialization.
*
*    Because variables are not initialized before __low_level_init() is called,
*    no access or call which relies on any variable can be used here.
*/
OS_INTERWORK int __low_level_init(void);
OS_INTERWORK int __low_level_init(void) {
  //
  // Initially disable watchdog
  //
  WD_DISABLE = 0x12345678;  // Write magic key as two consecutive write accesses.
  WD_DISABLE = 0x87654321;  // This should work, as long as interrupts are disabled.
  //
  // Init MMU and caches. This defines the virtual memory map, which is used during execution.
  // Code below fills all 4096 entries in ascending VAddr order.
  //
  OS_ARM_MMU_InitTT(_TranslationTable);
  //                                         Mode                      VAddr  PAddr  Size[MB]
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB,   0x000, 0x000, 0x001);    // Boot region, may be SDRAM if remapped. Then may be cacheable.
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_ILLEGAL, 0x001, 0x001, 0x01F);    // May be SDRAM, depends on external connecton, and linker script, not used in this sample application
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_C_B,     0x020, 0x020, 0x020);    // External SDRAM, Application program, size depends on hardware and linker script.
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_ILLEGAL, 0x040, 0x040, 0xBC0);    // SDRAM space, not used in this application
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB,   0xC00, 0xC00, 0x3C0);    // FPGA Slaves, 960MB
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB,   0xFC0, 0xFC0, 0x030);    // STM, 48MB
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB,   0xFF0, 0xFF0, 0x002);    // DAP, 2MB
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB,   0xFF2, 0xFF2, 0x002);    // LWFPGA Slaves, 2MB
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB,   0xFF4, 0xFF4, 0x00C);    // Peripherals, non cacheable, non bufferable, including OCSRAM ar 0xFFFF0000
  //
  // Now enable MMU, Caches and branch prediction unit
  //
  OS_ARM_MMU_Enable(_TranslationTable);
  OS_ARM_ICACHE_Enable();
  OS_ARM_DCACHE_Enable();
  return 1;
}

/*************************** End of file ****************************/

