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
Purpose : UART implementation

Additional information:

  Device : R7S72100
  Board  : R7S72100 RSK RZA1H

  Unit | UART   | Board connector
  ===============================
  0    | UART2  | USB (CN18)

*/

#include "BSP_UART.h"
#include "RTOS.h"      // For OS_INT_Enter()/OS_INT_Leave(). Remove this line and OS_INT_* functions if not using OS.

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

#define SYSTEM_FREQ          (399900000uL)
#define UART_FREQ            (SYSTEM_FREQ / 6uL)

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

//
// System, clock and standby control
//
#define CPG_BASE_ADDR        (0xFCFE0000uL)
#define CPG_FRQCR            (*(volatile unsigned short*)(CPG_BASE_ADDR + 0x0010uL))
#define CPG_FRQCR2           (*(volatile unsigned short*)(CPG_BASE_ADDR + 0x0014uL))
#define CPG_SYSCR3           (*(volatile unsigned char*) (CPG_BASE_ADDR + 0x0408uL))
#define CPG_STBCR4           (*(volatile unsigned char*) (CPG_BASE_ADDR + 0x0424uL))
//
// UART
//
#define PORT3_BASE_ADDR      (0xFCFE3000uL)
#define PORT3_PMC            (*(volatile unsigned short*)(PORT3_BASE_ADDR + 0x040CuL))
#define PORT3_PFCAE          (*(volatile unsigned short*)(PORT3_BASE_ADDR + 0x0A0CuL))
#define PORT3_PFCE           (*(volatile unsigned short*)(PORT3_BASE_ADDR + 0x060CuL))
#define PORT3_PFC            (*(volatile unsigned short*)(PORT3_BASE_ADDR + 0x050CuL))
#define PORT3_PIPC           (*(volatile unsigned short*)(PORT3_BASE_ADDR + 0x420CuL))

#define SCFI2_BASE_ADDR      (0xE8008000uL)
#define UART_BASE_ADDR       (SCFI2_BASE_ADDR)
#define UART_SCSMR           (*(volatile unsigned short*) (UART_BASE_ADDR + 0x0000uL))
#define UART_SCBRR           (*(volatile unsigned char*)  (UART_BASE_ADDR + 0x0004uL))
#define UART_SCSCR           (*(volatile unsigned short*) (UART_BASE_ADDR + 0x0008uL))
#define UART_SCFTDR          (*(volatile unsigned char*)  (UART_BASE_ADDR + 0x000CuL))
#define UART_SCFSR           (*(volatile unsigned short*) (UART_BASE_ADDR + 0x0010uL))
#define UART_SCFRDR          (*(volatile unsigned char*)  (UART_BASE_ADDR + 0x0014uL))
#define UART_SCFCR           (*(volatile unsigned short*) (UART_BASE_ADDR + 0x0018uL))
#define UART_SCSPTR          (*(volatile unsigned short*) (UART_BASE_ADDR + 0x0020uL))
#define UART_SCLSR           (*(volatile unsigned short*) (UART_BASE_ADDR + 0x0024uL))
#define UART_SCEMR           (*(volatile unsigned short*) (UART_BASE_ADDR + 0x0028uL))

#define UART_TX_SHIFT        (  0uL)
#define UART_RX_SHIFT        (  2uL)
#define UART_ERR_INT_ID      (230uL)
#define UART_RX_INT_ID       (231uL)
#define UART_TX_INT_ID       (232uL)

#define UART_FE_ERROR_SHIFT  (3)
#define UART_PE_ERROR_SHIFT  (2)
#define UART_RX_ERROR_SHIFT  (7)
#define UART_RDF_SHIFT       (1)
#define UART_RXRDY_SHIFT     (0)
#define UART_TEND_SHIFT      (6)
#define UART_TDFE_SHIFT      (5)
#define UART_RX_ERROR_MASK   (1uL << UART_RX_ERROR_SHIFT)
#define UART_RXRDY_MASK      (1uL << UART_RDF_SHIFT)
#define UART_TXRDY_MASK      (1uL << UART_TDFE_SHIFT)

#define UART_PRIO            (0xF0uL)  // Lowest priority for UART interrupts

#define BRG_VALUE(Baudrate)  ((UART_FREQ + (Baudrate) * 16uL) / ((Baudrate) * 32uL) - 1)

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

static void UART_RX_IRQHandler     (void);
static void UART_TX_IRQHandler     (void);
static void UART_RxError_IRQHandler(void);

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
*       UART_RX_IRQHandler()
*
*  Function description
*    UART Rx interrupt handler.
*/
void UART_RX_IRQHandler(void) {
  unsigned int  Data;
  unsigned char Status;

  Status = UART_SCFSR;                      // Examine interrupt identification register
  if (Status & UART_RXRDY_MASK) {           // Data received?
    Data        =   UART_SCFRDR;
    UART_SCFSR &= ~(UART_RXRDY_MASK);       // Clear Rx ready flag, reset interrupt pending
    if (Status & UART_RX_ERROR_MASK) {      // Any error ?
      UART_SCFSR &= ~(UART_RX_ERROR_MASK);  // Clear error flags
    } else if (_pfReadCB != 0) {
      _pfReadCB(0, Data);                   // Forward the received byte to the callback.
    }
  }
}

/*********************************************************************
*
*       UART_TX_IRQHandler()
*
*  Function description
*    UART Tx interrupt handler.
*/
void UART_TX_IRQHandler(void) {
  unsigned int Status;

  Status = UART_SCFSR;             // Examine status register
  if (Status & UART_TXRDY_MASK) {  // Check Tx status => Send next character
    //
    // Clear TX interrupt state
    //
    UART_SCSCR &= ~(1uL << 7);     // Disable Tx interrupt
    UART_SCFSR &= ~(UART_TXRDY_MASK);
    if (_pfWriteCB != 0) {
      _pfWriteCB(0);
    }
  }
}

/*********************************************************************
*
*       UART_RxError_IRQHandler()
*
*  Function description
*    OS UART Rx error interrupt handler
*/
static void UART_RxError_IRQHandler(void) {
  volatile unsigned int Dummy;
           unsigned int Status;

  Status = UART_SCFSR;                    // Examine status register
  if (Status & UART_RX_ERROR_MASK) {      // Any error ?
    Dummy       =   UART_SCFRDR;          // => Discard data
    OS_USE_PARA(Dummy);                   // Avoid compiler warning about variable set but not used
    UART_SCFSR &= ~(UART_RX_ERROR_MASK);  // Clear Error Flags
  }
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
  BSP_UART_USE_PARA(NumDataBits);
  BSP_UART_USE_PARA(Parity);
  BSP_UART_USE_PARA(NumStopBits);
  OS_INT_IncDI();
  //
  // Initially, the clock for UART module has to be enabled
  //
  CPG_STBCR4 &= ~(1u << 5);  // Enable clock for SCIF channel 2
  //
  // Now initialize the UART module
  //
  UART_SCSCR  = 0x0000u;             // Stop UART, select internal clock
  UART_SCFCR  = (0u << 0)            // Loopback mode disabled
              | (1u << 1)            // Reset Receive FIFO
              | (1u << 2)            // Reset Transmit FIFO
              | (0u << 3)            // Disable Modem control signals
              | (3u << 4)            // b5..b4: Transmit FIFO data trigger: 0 bytes, disable FIFO
              | (0u << 6)            // b7..b6;  Receive FIFO date trigger, 1 byte
              | (0u << 8)            // b10..b8: RTS active trigger, don't care, not used
              ;                      // All other bits reserved, always 0
  UART_SCFSR &= 0xFF6Eu;             // Reset error flags (Has to be Read AND Write operation)
  UART_SCLSR &= 0x0000u;             // Clear Overrun error, Has to be Read Modify write
  UART_SCSMR  = (0u << 0)            // b1..b0: Clock source divider. 0 => 1/1; 1 => 1/4; 2 => 1/16; 3 => 1/64
              | (0u << 2)            // b2:     Reserved, always 0
              | (0u << 3)            // b3:     1 Stop bit
              | (0u << 4)            // b4:     EVEN parity
              | (0u << 5)            // b5:     Parity disabled
              | (0u << 6)            // b6:     8 data bits
              | (0u << 7)            // b7:     Mode, async
              ;                      // All others reserved, set to 0
  UART_SCEMR  = (0u << 0)            // Async base clock oversampling 1/16
              | (0u << 7)            // Baud rate generator normal speed mode
              ;                      // All other bits reserved, always 0
  UART_SCBRR = BRG_VALUE(Baudrate);  // Set baudrate
  UART_SCFCR  = (0u << 0)            // Loopback mode disabled
              | (0u << 1)            // Release Reset for Receive FIFO
              | (0u << 2)            // Release Reset for Transmit FIFO
              | (0u << 3)            // Disable Modem control signals
              | (3u << 4)            // b5..b4: Transmit FIFO data trigger: 0 bytes, disable FIFO
              | (0u << 6)            // b7..b6;  Receive FIFO date trigger, 1 byte
              | (0u << 8)            // b10..b8: RTS active trigger, don't care, not used
              ;                      // All other bits reserved, always 0
  UART_SCSPTR = (1u << 0)            // Break output, output is high level
              | (1u << 0)            // Break bit is passed to TxD output
              ;                      // Handshake not used, all other bits 0
  //
  // Setup port for TxD
  //
  PORT3_PMC   |=  (1u << UART_TX_SHIFT);  // Alternate function TxD
  PORT3_PFCAE |=  (1u << UART_TX_SHIFT);  // Alternate function 6
  PORT3_PFCE  &= ~(1u << UART_TX_SHIFT);  // Alternate function 6
  PORT3_PFC   |=  (1u << UART_TX_SHIFT);  // Alternate function 6
  PORT3_PIPC  |=  (1u << UART_TX_SHIFT);  // Datat direction controlled by alternate function
  //
  // Setup port for RxD
  //
  PORT3_PMC   |=  (1u << UART_RX_SHIFT);  // Alternate function RxD
  PORT3_PFCAE &= ~(1u << UART_RX_SHIFT);  // Alternate function 4
  PORT3_PFCE  |=  (1u << UART_RX_SHIFT);  // Alternate function 4
  PORT3_PFC   |=  (1u << UART_RX_SHIFT);  // Alternate function 4
  PORT3_PIPC  |=  (1u << UART_RX_SHIFT);  // Datat direction controlled by alternate function
  //
  // Now enable Rx Tx and Interrupts
  //
  UART_SCSCR  = (0u << 0)   // b1..0: Clock select, Internal clock used
              | (0u << 2)   // b2:    Reserved, always 0
              | (0u << 3)   // b3:    Receive error interrupt, disabled
              | (1u << 4)   // b4:    Rx enabled
              | (1u << 5)   // b5:    Tx enabled
              | (1u << 6)   // b6:    Rx interrupt enabled
              | (0u << 7)   // b7:    Tx interrupt disabled per default
              ;             // b15..b8 all reserved, always 0
  //
  // Install interrupt handler and enable interrupts
  //
  OS_ARM_InstallISRHandler(UART_RX_INT_ID, UART_RX_IRQHandler);
  OS_ARM_ISRSetPrio       (UART_RX_INT_ID, UART_PRIO);
  OS_ARM_ClearPendingFlag (UART_RX_INT_ID);
  OS_ARM_EnableISR        (UART_RX_INT_ID);
  OS_ARM_InstallISRHandler(UART_TX_INT_ID, UART_TX_IRQHandler);
  OS_ARM_ISRSetPrio       (UART_TX_INT_ID, UART_PRIO);
  OS_ARM_ClearPendingFlag (UART_TX_INT_ID);
  OS_ARM_EnableISR        (UART_TX_INT_ID);
  OS_ARM_InstallISRHandler(UART_ERR_INT_ID, UART_RxError_IRQHandler);
  OS_ARM_ISRSetPrio       (UART_ERR_INT_ID, UART_PRIO);
  OS_ARM_ClearPendingFlag (UART_ERR_INT_ID);
  OS_ARM_EnableISR        (UART_ERR_INT_ID);
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
  UART_SCFTDR = Data;
  UART_SCSCR |= (1u << 7);  // b7: Enable Tx interrupt
}

/*************************** End of file ****************************/
