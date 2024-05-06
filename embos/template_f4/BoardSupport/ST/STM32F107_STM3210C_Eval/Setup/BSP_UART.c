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

  Device : STM32F107
  Board  : ST STM3210C Eval

  Unit | UART   | Board connector
  ===============================
  0    | USART2 | USART (CN6)

*/

#include "BSP_UART.h"
#include "RTOS.h"        // For OS_INT_Enter()/OS_INT_Leave(). Remove this line and OS_INT_* functions if not using OS.
#include "stm32f10x.h"   // Device specific header file, contains CMSIS defines.

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

#define BSP_UART_BAUDRATE     (38400)
#define BSP_UART_CLOCK        (SystemCoreClock / 2)
#define BSP_UART_IRQHandler   (USART2_IRQHandler)

#define USART_BASE_ADDR       (0x40004400u)
#define USART_BRR_VALUE       (BSP_UART_CLOCK / BSP_UART_BAUDRATE)
#define USART_SR              (*(volatile unsigned long*)(USART_BASE_ADDR + 0x00u))
#define USART_DR              (*(volatile unsigned long*)(USART_BASE_ADDR + 0x04u))
#define USART_BRR             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x08u))
#define USART_CR1             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x0Cu))
#define USART_CR2             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x10u))
#define USART_CR3             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x14u))
#define USART_GTPR            (*(volatile unsigned long*)(USART_BASE_ADDR + 0x18u))

#define GPIOD_CRL             (*(volatile unsigned long*)(0x40011400u))

#define RCC_BASE_ADDR         (0x40021000u)
#define RCC_APB2ENR           (*(volatile unsigned long*)(RCC_BASE_ADDR + 0x18u))
#define RCC_APB1ENR           (*(volatile unsigned long*)(RCC_BASE_ADDR + 0x1Cu))

#define AFIO_BASE_ADDR        (0x40010000u)
#define AFIO_MAPR             (*(volatile unsigned long*)(AFIO_BASE_ADDR + 0x04u))

#define US_RXRDY              (0x20u)   // RXNE
#define US_TXEMPTY            (0x80u)   // TXE
#define USART_RX_ERROR_FLAGS  (0x0Fu)   // ORE/NE/FE/PE
#define RCC_USART2EN          ((1uL << 17))

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
  unsigned char Status;
  unsigned char Data;
  unsigned int  Unit;

  OS_INT_EnterNestable();
  Status = USART_SR;                               // Read status register.
  Unit   = 0;
  //
  // Handle Rx.
  //
  do {
    if (Status & US_RXRDY) {                       // Data received?
      Data = USART_DR;
      if ((Status & USART_RX_ERROR_FLAGS) == 0) {  // If no errors
        if (_pfReadCB != NULL) {
          _pfReadCB(Unit, Data);                   // Process actual byte
        }
      } else {
        Data = Data + 1;
      }
    }
    Status = USART_SR;                             // Examine current status
  } while (Status & US_RXRDY);
  //
  // Handle Tx.
  //
  if (Status & US_TXEMPTY) {
    if (_pfWriteCB != NULL && _pfWriteCB(Unit)) {  // No more characters to send ?
      USART_CR1 &= ~0x40uL;                        // Disable further Tx interrupts
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
  //
  // Unused parameters. Hard coded USART settings.
  //
  BSP_UART_USE_PARA(Unit);
  BSP_UART_USE_PARA(Baudrate);
  BSP_UART_USE_PARA(NumDataBits);
  BSP_UART_USE_PARA(Parity);
  BSP_UART_USE_PARA(NumStopBits);
  //
  // Setup clocks, GPIO ports and NVIC IRQs.
  //
  RCC_APB1ENR |= RCC_USART2EN;         // Enable USART2 clock
  RCC_APB2ENR |= (1 << 5) | (1 << 0);  // Enable GPIO port needed for USART, enable alternate function
  AFIO_MAPR   |= (1 << 3);             // Remap USART2
  GPIOD_CRL    = (GPIOD_CRL & 0xF00FFFFFuL) | 0x04B00000uL;
  NVIC_SetPriority(USART2_IRQn, (1 << __NVIC_PRIO_BITS) - 2);
  NVIC_EnableIRQ(USART2_IRQn);
  //
  // Initialize USART
  //
  USART_CR2   = 0x200;
  USART_CR1   = 0x2C;
  USART_CR3   = 0x00;
  USART_BRR   = USART_BRR_VALUE;
  USART_CR1  |= 0x2000;                // Enable uart
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
  NVIC_DisableIRQ(USART2_IRQn);
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
  while ((USART_SR & (1uL << 7)) == 0);
  USART_DR   = Data;
  USART_CR1 |= 0x40;  // Enable Tx interrupt
}

/*************************** End of file ****************************/
