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
File    : BSP_UART.c
Purpose : UART implementation

Additional information:

  Device : 5CSEBA6 with Altera Cortex-A9 MPCore
  Board  : Intel Socrates evaluation kit

  Unit | UART   | Board connector
  ===============================
  0    | UART0  | Virtual COM (P6)

*/

#include "BSP_UART.h"
#include "RTOS.h"      // For OS_INT_Enter()/OS_INT_Leave(). Remove this line and OS_INT_* functions if not using OS.

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/
#define OS_PCLK_UART                  ((800000000)/8)
#define UART_INT_PRIO                 (240u)
#define UART0_BASE_ADDR               (0xFFC02000u)
#define UART1_BASE_ADDR               (0xFFC03000u)

//
// Assign UART to port and peripheral
//
#define OS_UART_BASE_ADDR             (UART0_BASE_ADDR)
#define OS_UART_INT_ID                (194)
#define OS_UART_PER_RESET_BIT         (16)

#define OS_UART_RBR                   (*(volatile OS_U8* )(OS_UART_BASE_ADDR + 0x00u))
#define OS_UART_THR                   (*(volatile OS_U8* )(OS_UART_BASE_ADDR + 0x00u))
#define OS_UART_IER                   (*(volatile OS_U32*)(OS_UART_BASE_ADDR + 0x04u))
#define OS_UART_IIR                   (*(volatile OS_U32*)(OS_UART_BASE_ADDR + 0x08u))
#define OS_UART_FCR                   (*(volatile OS_U8* )(OS_UART_BASE_ADDR + 0x08u))
#define OS_UART_LCR                   (*(volatile OS_U8* )(OS_UART_BASE_ADDR + 0x0Cu))
#define OS_UART_LSR                   (*(volatile OS_U8* )(OS_UART_BASE_ADDR + 0x14u))
#define OS_UART_SCR                   (*(volatile OS_U8* )(OS_UART_BASE_ADDR + 0x1Cu))
#define OS_UART_DLL                   (*(volatile OS_U8* )(OS_UART_BASE_ADDR + 0x00u))
#define OS_UART_DLM                   (*(volatile OS_U8* )(OS_UART_BASE_ADDR + 0x04u))

#define BAUDDIVIDE                    ((OS_PCLK_UART + (38400 * 8)) / (38400 * 16))

#define RSTMGR_BASE_ADDR              (0xFFD05000u)
#define OS_UART_PERIPHERAL_RESET_REG  (*(volatile OS_U32*)(RSTMGR_BASE_ADDR + 0x14u))

#define RX_FULL_INT_ENABLE_BIT        (0)
#define TX_EMPTY_INT_ENABLE_BIT       (1)
#define RX_ERROR_INT_ENABLE_BIT       (2)
#define INT_PENDING_BIT               (0)

#define UART_INT_MASK                 (0x0Eu)
#define UART_ERROR_INT_STATUS         (0x06u)
#define UART_RX_INT_STATUS            (0x04u)
#define UART_TX_INT_STATUS            (0x02u)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static BSP_UART_TX_CB* _pfWriteCB;
static BSP_UART_RX_CB* _pfReadCB;

/*********************************************************************
*
*       Prototypes
*
*  Declare ISR handler here to avoid "no prototype" warning.
*  They are not declared in any CMSIS header.
*
**********************************************************************
*/

#if defined(__cplusplus)
  extern "C" {                // Make sure we have C-declarations in C++ programs.
#endif

void UART_RX_TX_IRQHandler(void);

#if defined(__cplusplus)
}                             // Make sure we have C-declarations in C++ programs.
#endif

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _SetBaudrate()
*
*  Function description
*    Configures the UART baud rate.
*
*  Parameters
*    Unit    : Unit number (typically zero-based).
*    Baudrate: Baud rate to configure [Hz].
*/
static void _SetBaudrate(unsigned int Unit, unsigned long Baudrate) {
  BSP_UART_USE_PARA(Unit);
  BSP_UART_USE_PARA(Baudrate);
}

/*********************************************************************
*
*       Global functions, IRQ handler
*
**********************************************************************
*/

/*********************************************************************
*
*       UART_RX_TX_IRQHandler()
*
*  Function description
*    UART Rx & Tx interrupt handler.
*
*  Additional information
*    Needs to inform the OS that we are in interrupt context.
*/
void UART_RX_TX_IRQHandler(void) {
  volatile OS_U32 Dummy;
           OS_U32 Unit;
           OS_U8  Status;
           OS_U8  Data;

  OS_INT_Enter();
  Status = 0u;                                              // Dummy status register value. Read status register here.
  Unit   = 0u;                                              // Identify UART unit if multiple units are used.
  Status = OS_UART_IIR & UART_INT_MASK;                     // Examine interrupt status
  if (Status == UART_ERROR_INT_STATUS) {                    // Error pending ?
    Dummy = OS_UART_RBR;                                    // Discard data
    Dummy = OS_UART_LSR;                                    // reset error
  } else if (Status == UART_RX_INT_STATUS) {                // Data received?
    if (_pfReadCB != NULL) {
      _pfReadCB(Unit, OS_UART_RBR);                         // Forward the received byte to the callback.
    }
  } else if (Status == UART_TX_INT_STATUS) {                // Tx interrupt pending?
    if ((_pfWriteCB != NULL) && (_pfWriteCB(Unit) != 0)) {  // No more character sent?
      OS_UART_IER &= ~(1uL << TX_EMPTY_INT_ENABLE_BIT);     // Disable Tx interrupts
    }
  }
  OS_INT_Leave();
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       BSP_UART_Init()
*
*  Function description
*    Initializes the UART peripheral.
*
*  Parameters
*    Unit       : Unit number (typically zero-based).
*    Baudrate   : Baud rate to configure [Hz].
*    NumDataBits: Number of data bits to use.
*    Parity     : One of the following values:
*                   * BSP_UART_PARITY_NONE
*                   * BSP_UART_PARITY_ODD
*                   * BSP_UART_PARITY_EVEN
*    NumStopBits: Number of stop bits to use.
*/
void BSP_UART_Init(unsigned int Unit, unsigned long Baudrate, unsigned char NumDataBits, unsigned char Parity, unsigned char NumStopBits) {
  BSP_UART_USE_PARA(Unit);
  BSP_UART_USE_PARA(Baudrate);
  BSP_UART_USE_PARA(NumDataBits);
  BSP_UART_USE_PARA(Parity);
  BSP_UART_USE_PARA(NumStopBits);
  OS_INT_IncDI();
  //
  // Ensure, reset for UART is released, clear reset bit in Reset manager
  //
  OS_UART_PERIPHERAL_RESET_REG &= ~(1u << OS_UART_PER_RESET_BIT);
  //
  // Initialize the selected UART using project settings and defines
  //
  OS_UART_IER  = 0x00u;     // Initially disable all interrupts
  OS_UART_LCR  = 0x80u;     // Set DLAB to initialize Baudrate generator
  OS_UART_DLL  = (BAUDDIVIDE & 0xFFu);
  OS_UART_DLM  = ((BAUDDIVIDE >> 8) & 0xFFu);
  OS_UART_LCR &= ~0x80u;    // reset DLAB to lock baudrate generator access
  OS_UART_LCR  = 0x03u      // 8 data bits
               | (0u << 2)  // 1 stop bit
               | (0u << 3)  // NO parity
               | (0u << 4)  // Parity setting (bit 5:4) does not care
               | (0u << 6)  // Disable Break transmission
               | (0u << 7)  // Clear DLAB
               ;
  OS_UART_FCR  = (0u << 0)  // Disable FIFO (with embOS we send / receive byte by byte)
               | (1u << 1)  // Reset RxFIFO
               | (1u << 2)  // Reset TxFIFO
               | (0u << 6)  // Set trigger-level for RxFIFO
               ;
  //
  // Install OS UART interrupt handler
  //
  OS_ARM_InstallISRHandler(OS_UART_INT_ID, UART_RX_TX_IRQHandler);  // OS UART interrupt vector.
  OS_ARM_ISRSetPrio(OS_UART_INT_ID, UART_INT_PRIO);
  OS_ARM_EnableISR(OS_UART_INT_ID);                                 // Enable UART interrupt in GIC
  OS_UART_IER |= (1u << RX_FULL_INT_ENABLE_BIT);                    // Enable Rx interrupts
  OS_INT_DecRI();
}

/*********************************************************************
*
*       BSP_UART_DeInit()
*
*  Function description
*    De-initializes the UART peripheral.
*
*  Parameters
*    Unit: Unit number (typically zero-based).
*/
void BSP_UART_DeInit(unsigned int Unit) {
  BSP_UART_USE_PARA(Unit);
}

/*********************************************************************
*
*       BSP_UART_SetBaudrate()
*
*  Function description
*    Configures/changes the UART baud rate.
*
*  Parameters
*    Unit    : Unit number (typically zero-based).
*    Baudrate: Baud rate to configure [Hz].
*/
void BSP_UART_SetBaudrate(unsigned int Unit, unsigned long Baudrate) {
  _SetBaudrate(Unit, Baudrate);
}

/*********************************************************************
*
*       BSP_UART_SetReadCallback()
*
*  Function description
*    Sets the callback to execute upon an Rx interrupt.
*
*  Parameters
*    Unit: Unit number (typically zero-based).
*    pf  : Callback to execute.
*/
void BSP_UART_SetReadCallback(unsigned Unit, BSP_UART_RX_CB* pf) {
  BSP_UART_USE_PARA(Unit);
  _pfReadCB = pf;
}

/*********************************************************************
*
*       BSP_UART_SetWriteCallback()
*
*  Function description
*    Sets the callback to execute upon a Tx interrupt.
*
*  Parameters
*    Unit: Unit number (typically zero-based).
*    pf  : Callback to execute.
*/
void BSP_UART_SetWriteCallback(unsigned int Unit, BSP_UART_TX_CB* pf) {
  BSP_UART_USE_PARA(Unit);
  _pfWriteCB = pf;
}

/*********************************************************************
*
*       BSP_UART_Write1()
*
*  Function description
*    Sends one byte via UART.
*
*  Parameters
*    Unit: Unit number (typically zero-based).
*    Data: (First) data byte to send.
*
*  Additional information
*    The first byte of a transfer is typically sent from application
*    context. Further bytes of the transfer are then sent from the
*    Tx interrupt handler by also calling this function from interrupt
*    context.
*/
void BSP_UART_Write1(unsigned int Unit, unsigned char Data) {
  BSP_UART_USE_PARA(Unit);
  OS_UART_THR  = Data;                             // Send character
  OS_UART_IER |= (1u << TX_EMPTY_INT_ENABLE_BIT);  // Enable Tx interrupts
}

/*************************** End of file ****************************/
