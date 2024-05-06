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
File    : RTOSInit_iMX6U5.c
Purpose : Initializes and handles the hardware for embOS

Literature:
  [1]  Generic Interrupt Controller (GIC-390) Errata Notice
       (\\fileserver\Techinfo\Company\ARM\GIC_GenericInterruptController\GIC_390_Errata_Notice_v6.pdf)
  [2]  i.MX 6Solo/6DualLite Applications Processor Reference Manual
       (\\fileserver\Techinfo\Company\NXP\MCU\iMX(ARM)\iMX6DL\IMX6SDLRM_Rev3.pdf)
*/

#include "RTOS.h"

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
#define OS_TIMER_FREQ    (396000000u)
#define OS_TICK_FREQ     (1000u)
#define OS_INT_FREQ      (OS_TICK_FREQ)
#define OS_TIMER_RELOAD  ((OS_TIMER_FREQ / OS_TICK_FREQ) - 1u)

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
//
// L2 Cache controller
//
#define L2CACHE_BASE_ADDR          (0x00A02000u)

//
// Core private timer
//
#define ARM_MP_BASE_ADDR           (0x00A00000u)
#define CORE_PRIV_TIMER_BASE_ADDR  (ARM_MP_BASE_ADDR + 0x0600u)
#define CORE_PRIV_TIMER_LOAD       (*(volatile OS_U32*)(CORE_PRIV_TIMER_BASE_ADDR + 0x00u))
#define CORE_PRIV_TIMER_COUNT      (*(volatile OS_U32*)(CORE_PRIV_TIMER_BASE_ADDR + 0x04u))
#define CORE_PRIV_TIMER_CONTROL    (*(volatile OS_U32*)(CORE_PRIV_TIMER_BASE_ADDR + 0x08u))
#define CORE_PRIV_TIMER_STATUS     (*(volatile OS_U32*)(CORE_PRIV_TIMER_BASE_ADDR + 0x0Cu))
#define CORE_PRIV_TIMER_INT_ID     (29u)

//
// Generic interrupt controller
//
#define GICC_BASE_ADDR             (ARM_MP_BASE_ADDR + 0x0100u)
#define GICC_HPPIR                 (*(volatile OS_U32*)(GICC_BASE_ADDR + 0x018u))
#define GICD_BASE_ADDR             (ARM_MP_BASE_ADDR + 0x1000u)
#define GICD_IPRIORITY             (*(volatile OS_U32*)(GICD_BASE_ADDR + 0x400u))

#define NUM_INT_SOURCES            (159u)

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/
extern const OS_ARM_L2CACHE_API OS_L2CACHE_L2C310;  // IMX6SDLRM [2], page 537
extern       unsigned int       __vector;           // Start of vector table, defined in startup file

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

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
  static OS_U32 _TranslationTable[0x1000] __attribute__ ((aligned (16384), section (".non_init._TranslationTable")));
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
  static OS_ISR_HANDLER* _apIsrHandler[NUM_INT_SOURCES] __attribute__ ((aligned (4), section (".non_init._apIsrHandler")));
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
*    Initialize interrupt controller by setting default vectors
*    and clearing all interrupts
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
  return (OS_TIMER_RELOAD - CORE_PRIV_TIMER_COUNT);
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
  return OS_ARM_IsPending(CORE_PRIV_TIMER_INT_ID);
}

/*********************************************************************
*
*       _OS_ISR_Tick()
*
*  Function description
*    This is the hardware timer exception handler.
*/
static void _OS_ISR_Tick(void) {
  CORE_PRIV_TIMER_STATUS = (1u << 0);  // Clear timer interrupt pending flag
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
  OS_ARM_InstallISRHandler(CORE_PRIV_TIMER_INT_ID, _OS_ISR_Tick);
  OS_ARM_ISRSetPrio(CORE_PRIV_TIMER_INT_ID, 0xFFu);              // Set to lowest level, ALL BITS set
  TickPrio  = OS_ARM_ISRSetPrio(CORE_PRIV_TIMER_INT_ID, 0xFFu);  // Read back priority to examine relevant preemption-level-bits
  TickPrio -= 1;                                                 // Set to low preemption level, 1 level higher than lowest
  OS_ARM_ISRSetPrio(CORE_PRIV_TIMER_INT_ID, TickPrio);
  OS_ARM_EnableISR(CORE_PRIV_TIMER_INT_ID);
  CORE_PRIV_TIMER_CONTROL = 0x00u;                               // Stop Timer, set Prescaler to 1
  CORE_PRIV_TIMER_LOAD    = OS_TIMER_RELOAD;                     // Set timer interval
  CORE_PRIV_TIMER_CONTROL = (1u << 0)                            // Enable Timer
                          | (1u << 1)                            // Set auto reload mode
                          | (1u << 2);                           // Enable timer interrupt
  //
  // Inform embOS about the timer settings
  //
  {
    OS_SYSTIMER_CONFIG SysTimerConfig = {OS_TIMER_FREQ, OS_INT_FREQ, OS_TIMER_UPCOUNTING, _OS_GetHWTimerCycles, _OS_GetHWTimer_IntPending};
    OS_TIME_ConfigSysTimer(&SysTimerConfig);
  }
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
  volatile OS_U32 Dummy;
  OS_U32          int_id;
  OS_U32          icdipr0_value;

  Dummy = GICC_HPPIR;                              // Erratum 801120 [1] - ensure subsequent ICCIAR data will be correct
  OS_USE_PARA(Dummy);                              // Avoid compiler warning about set but unused variable
  int_id = OS_GIC_AckInt();                        // Acknowledge the interrupt and read the interrupt ID
  //
  // Read data of 0x0 (SGI ID0 from CPU0) is possibly affected by 733075
  // Interrupt IDs 0x3FE and 0x3FF are also possibly affected
  //
  if ((int_id == 0u) || (int_id  >= 0x3FEu)) {
    //
    // Erratum 733075 - prevent possible CPU interface deadlock (ARM Errata Notice, PR258-GENC-008719 v6.0).
    // Assumes SGI ID0 from CPU0 is not used as suggested by ARM.
    //
    icdipr0_value  = GICD_IPRIORITY;               // One way to get the value for the workaround write is to read it first
    GICD_IPRIORITY = icdipr0_value;
    __asm volatile("dsb");                         // Ensure write completes before any future ICCHPIR or ICCIAR reads
  }
  if ((int_id == 0x3FEu) || (int_id == 0x3FFu)) {  // Check for spurious interrupts
     _SpuriousIrqHandler();
  } else {
    if (int_id < NUM_INT_SOURCES) {
      pISR = _apIsrHandler[int_id];                // Get interrupt vector handler
      OS_INT_Enter();
      pISR();                                      // Call interrupt service routine
      OS_INT_Leave();
    }
    OS_GIC_EOI(int_id);                            // End interrupt
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
  // Init MMU and caches. This defines the virtual memory map, which is used during execution.
  // Code below fills all 4096 entries in ascending VAddr order.
  //
  OS_ARM_MMU_InitTT(_TranslationTable);
  //                                         Mode                                Access permissions        VAddr  PAddr  Size[MB]
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED,                            0x000, 0x100, 0x001);  // MMDC-DDR, mapped at 0x00
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,                                          0x001, 0x001, 0x008);  // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_WRITE_BACK_ALLOC,                            0x009, 0x009, 0x001);  // OCRAM - 256 KByte at 0x00900000
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED | OS_ARM_MMU_EXECUTE_NEVER, 0x00A, 0x00A, 0x001);  // ARM MP + LC310
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,                                          0x00B, 0x00B, 0x015);  // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED | OS_ARM_MMU_EXECUTE_NEVER, 0x020, 0x020, 0x001);  // Peripheral AIPS-1
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED | OS_ARM_MMU_EXECUTE_NEVER, 0x021, 0x021, 0x001);  // Peripheral AIPS-2
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,                                          0x022, 0x022, 0x004);  // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED,                            0x026, 0x026, 0x004);  // IPU
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,                                          0x02A, 0x02A, 0x056);  // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED,                            0x080, 0x080, 0x080);  // 128 MB EIM (NOR/SRAM)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_WRITE_BACK_ALLOC,                            0x100, 0x100, 0x400);  // MMDC-DDR, write-back cached, 10124 MByte
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED,                            0x500, 0x100, 0x400);  // MMDC-DDR, not cached (used e.g. for emUSB), 10124 MByte
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_WRITE_THROUGH,                               0x900, 0x100, 0x400);  // MMDC-DDR, write-through cached, 10124 MByte
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,                                          0xD00, 0xD00, 0x300);  // Unmapped area
  //
  // Set API functions and base address for L2 Cache
  //
  OS_ARM_AddL2Cache(&OS_L2CACHE_L2C310, (void*)L2CACHE_BASE_ADDR);
  //
  // Now enable MMU, Caches and branch prediction unit
  //
  OS_ARM_MMU_Enable(_TranslationTable);
  OS_ARM_ICACHE_Enable();
  OS_ARM_DCACHE_Enable();
  return 1;
}

/*************************** End of file ****************************/
