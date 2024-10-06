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

-------------------------- END-OF-HEADER -----------------------------
Purpose : UART implementation

Additional information:

  Device : LPC1768
  Board  : Keil MCB1700

  Unit | UART   | Board connector
  ===============================
  0    | UART0  | COM0

*/

#include "BSP_UART.h"
#include "RTOS.h"      // For OS_INT_Enter()/OS_INT_Leave(). Remove this line and OS_INT_* functions if not using OS.
#include "LPC17xx.h"   // Device specific header file, contains CMSIS defines.

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

#define BSP_UART_FREQUENCY       (SystemCoreClock / 4)
#define BSP_UART_BAUDRATE        (38400)
#define BSP_UART_IRQHandler      (UART0_IRQHandler)
#define BSP_UART_IRQn            (UART0_IRQn)

#define BAUDDIVIDE               ((BSP_UART_FREQUENCY + BSP_UART_BAUDRATE * 8L) / (BSP_UART_BAUDRATE * 16L))
#define PCONP                    (*(volatile unsigned long*)(0x400FC0C4uL))
#define PINCON_BASE_ADDR         (0x4002C000uL)
#define PINSEL0                  (*(volatile unsigned long*)(PINCON_BASE_ADDR + 0x00))

/****** UART sfrs used for embOSView communication ******************/
#define UART0_BASE_ADDR          (0x4000C000uL)
#define UART_RBR                 (*(volatile unsigned char*)(UART0_BASE_ADDR + 0x00))
#define UART_THR                 (*(volatile unsigned char*)(UART0_BASE_ADDR + 0x00))
#define UART_IER                 (*(volatile unsigned char*)(UART0_BASE_ADDR + 0x04))
#define UART_IIR                 (*(volatile unsigned char*)(UART0_BASE_ADDR + 0x08))
#define UART_FCR                 (*(volatile unsigned char*)(UART0_BASE_ADDR + 0x08))
#define UART_LCR                 (*(volatile unsigned char*)(UART0_BASE_ADDR + 0x0C))
#define UART_LSR                 (*(volatile unsigned char*)(UART0_BASE_ADDR + 0x14))
#define UART_DLL                 (*(volatile unsigned char*)(UART0_BASE_ADDR + 0x00))
#define UART_DLM                 (*(volatile unsigned char*)(UART0_BASE_ADDR + 0x04))

#define RX_FULL_INT_ENABLE_BIT   (0)
#define TX_EMPTY_INT_ENABLE_BIT  (1)
#define RX_ERROR_INT_ENABLE_BIT  (2)
#define INT_PENDING_BIT          (0)

#define UART_INT_MASK            (0x0Eu)
#define UART_ERROR_INT_STATUS    (0x06u)
#define UART_RX_INT_STATUS       (0x04u)
#define UART_TX_INT_STATUS       (0x02u)

#define UART_PCONP_BIT           (3)
#define PINSEL_UART_MODE_RX_POS  (6)
#define PINSEL_UART_MODE_TX_POS  (4)
#define PINSEL_UART_MODE_RX_VAL  (1)
#define PINSEL_UART_MODE_TX_VAL  (1)

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

void BSP_UART_IRQHandler(void);

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
*       BSP_UART_IRQHandler()
*
*  Function description
*    UART Rx & Tx interrupt handler.
*
*  Additional information
*    Needs to inform the OS that we are in interrupt context.
*/
void BSP_UART_IRQHandler(void) {
  volatile unsigned long Dummy;
           unsigned int  Unit;
           unsigned char Status;
           unsigned char Data;

  OS_INT_EnterNestable();
  Status = UART_IIR;                                        // Read status register.
  Unit   = 0u;                                              // Identify UART unit if multiple units are used.
  //
  // Handle error
  //
  if ((Status & UART_INT_MASK) == UART_ERROR_INT_STATUS) {  // Error pending ?
    Dummy = UART_RBR;                                       // Discard data
    Dummy = UART_LSR;                                       // reset error
   (void)Dummy;
    OS_INT_LeaveNestable();
    return;
  }
  //
  // Handle Tx. Only accept Tx interrupts if we have sent something.
  // Ignore Tx empty flag when we process Rx interrupts.
  //
  if ((Status & UART_INT_MASK) == UART_TX_INT_STATUS) {
    if (_pfWriteCB != NULL) {
      if (_pfWriteCB(Unit) != 0) {                          // Receive the next byte to send.
        UART_IER &= ~(1uL << TX_EMPTY_INT_ENABLE_BIT);      // Disable Tx interrupts
      }
    }
  }
  //
  // Handle Rx.
  //
  if ((Status & UART_INT_MASK) == UART_RX_INT_STATUS) {     // Data received?
    Data = UART_RBR;                                        // Read received data.
    if (_pfReadCB != NULL) {
      _pfReadCB(Unit, Data);                                // Forward the received byte to the callback.
    }
  }
  OS_INT_LeaveNestable();
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
  //
  // Setup Port-Mode to alternate function
  //
  PINSEL0 &= ~(0x03uL << PINSEL_UART_MODE_RX_POS);
  PINSEL0 &= ~(0x03uL << PINSEL_UART_MODE_TX_POS);
  PINSEL0 |=  (PINSEL_UART_MODE_RX_VAL << PINSEL_UART_MODE_RX_POS);
  PINSEL0 |=  (PINSEL_UART_MODE_TX_VAL << PINSEL_UART_MODE_TX_POS);

  PCONP       |= (1uL << UART_PCONP_BIT);          // Enable UART unit
  UART_IER  = 0x00;                                // Initially disable all interrupts
  UART_LCR  = 0x80;                                // Set DLAB to initialize Baudrate generator
  UART_DLL  = (BAUDDIVIDE & 0xFF);
  UART_DLM  = ((BAUDDIVIDE >> 8) & 0xFF);
  UART_LCR &= ~0x80;                               // reset DLAB to lock baudrate generator access
  UART_LCR  = 0x03                                 // 8 data bits
               | (0 << 2)                          // 1 stop bit
               | (0 << 3)                          // NO parity
               | (0 << 4)                          // Parity setting (bit 5:4) does not care
               | (0 << 6)                          // Disable Break transmission
               | (0 << 7);                         // Clear DLAB
  UART_FCR  = (1uL << 0)                           // Enable FIFO (according to documentation)
               | (1uL << 1)                        // Reset RxFIFO
               | (1uL << 2)                        // Reset TxFIFO
               | (0 << 6)                          // Set trigger-level for RxFIFO
               ;
  //
  // Configure baud rate.
  //
  BSP_UART_USE_PARA(Baudrate);
  //
  // Configure NumDataBits, Parity and NumStopBits.
  //
  BSP_UART_USE_PARA(NumDataBits);
  BSP_UART_USE_PARA(Parity);
  BSP_UART_USE_PARA(NumStopBits);
  //
  // Setup UART interrupt(s).
  //
  NVIC_SetPriority(BSP_UART_IRQn, (1 << __NVIC_PRIO_BITS) - 2);
  NVIC_EnableIRQ(BSP_UART_IRQn);
  UART_IER |= (1uL << RX_FULL_INT_ENABLE_BIT);     // Enable Rx interrupts
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
  NVIC_DisableIRQ(BSP_UART_IRQn);
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
  UART_THR  = Data;                              // Send character
  UART_IER |= (1uL << TX_EMPTY_INT_ENABLE_BIT);  // Enable Tx interrupts
}

/*************************** End of file ****************************/
