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

  Device : STM32L152
  Board  : IAR STM32L152VB Starter Kit

  Unit | UART   | Board connector
  ===============================
  0    | USART2 | RS232_2

*/

#include "BSP_UART.h"
#include "RTOS.h"        // For OS_INT_Enter()/OS_INT_Leave(). Remove this line and OS_INT_* functions if not using OS.
#include "stm32l1xx.h"   // Device specific header file, contains CMSIS defines.

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

#define BSP_UART_CLOCK        (SystemCoreClock)
#define BSP_UART_IRQHandler   (USART2_IRQHandler)
#define BSP_UART_IRQn         (USART2_IRQn)

#define USART_BASE_ADDR       (0x40004400u)  // USART2
#define USART_SR              (*(volatile unsigned long*)(USART_BASE_ADDR + 0x00u))
#define USART_DR              (*(volatile unsigned long*)(USART_BASE_ADDR + 0x04u))
#define USART_BRR             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x08u))
#define USART_CR1             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x0Cu))
#define USART_CR2             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x10u))
#define USART_CR3             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x14u))

#define RCC_BASE_ADDR         (0x40023800u)
#define RCC_AHBENR            (*(volatile unsigned long*)(RCC_BASE_ADDR + 0x1Cu))
#define RCC_APB1ENR           (*(volatile unsigned long*)(RCC_BASE_ADDR + 0x24u))

#define GPIO_BASE_ADDR        (0x40020C00u)  // GPIOD
#define GPIO_MODER            (*(volatile unsigned long*)(GPIO_BASE_ADDR + 0x00u))
#define GPIO_ODR              (*(volatile unsigned long*)(GPIO_BASE_ADDR + 0x14u))
#define GPIO_AFRL             (*(volatile unsigned long*)(GPIO_BASE_ADDR + 0x20u))
#define GPIO_AFRH             (*(volatile unsigned long*)(GPIO_BASE_ADDR + 0x24u))

#define US_RXRDY              (0x20u)        // RXNE
#define US_TXEMPTY            (0x80u)        // TXE
#define USART_RX_ERROR_FLAGS  (0x0Fu)        // ORE/NE/FE/PE

#define GPIO_EN               (3u)
#define RS232_EN              (0u)
#define GPIO_AF7              (7u)
#define GPIO_MODER_MASK       (0x03u)
#define GPIO0_MODER           (0u)
#define GPIO5_MODER           (10u)
#define GPIO6_MODER           (12u)
#define GPIO_MODER_OUTPUT     (1u)
#define GPIO_MODER_ALTFUNC    (2u)
#define GPIO_AFRL_MASK        (0x0Fu)
#define GPIO5_AFRL            (20u)
#define GPIO6_AFRL            (24u)


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
  unsigned int bWasEnabled;

  bWasEnabled = 0;
  if (Unit == 0) {
    if (USART_CR1 & (1uL << 13)) {
      bWasEnabled = 1;
      USART_CR1 &= ~(1uL << 13);            // Disable USART.
    }
    USART_BRR = BSP_UART_CLOCK / Baudrate;  // Set baudrate.
    if (bWasEnabled == 1) {
      USART_CR1 |=  (1uL << 13);            // Enable USART.
    }
  }
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
  unsigned int  Status;
  unsigned char Data;

  OS_EnterNestableInterrupt();
  Status = USART_SR;                        // Examine status register
  //
  // Handle Rx.
  //
  do {
    if (Status & US_RXRDY) {                // Data received?
      Data = USART_DR;
      if (Status & USART_RX_ERROR_FLAGS) {  // Any error ?
      } else if (_pfReadCB) {
        _pfReadCB(0, (unsigned char)Data);
      }
    }
    Status = USART_SR;                      // Examine current status
  } while (Status & US_RXRDY);
  //
  // Handle Tx.
  //
  if ((Status & US_TXEMPTY) && ((USART_CR1 & 0x40uL) != 0)) {
    if (_pfWriteCB) {
      if (_pfWriteCB(0)) {                  // No more characters to send ?
        USART_CR1 &= ~0x40uL;               // Disable further Tx interrupts
      }
    }
  }
  OS_LeaveNestableInterrupt();
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
  BSP_UART_USE_PARA(NumDataBits);
  BSP_UART_USE_PARA(Parity);
  BSP_UART_USE_PARA(NumStopBits);
  if (Unit == 0) {
    //
    // Set port pins for Uart2 (Tx: PD5, Rx: PD6)
    //
    RCC_AHBENR  |= (1u << GPIO_EN);                                                                                                   // Enable GPIOD clock
    GPIO_MODER &= ~((GPIO_MODER_MASK << GPIO0_MODER) | (GPIO_MODER_MASK << GPIO5_MODER) | (GPIO_MODER_MASK << GPIO5_MODER));          // Clear Mode for Rx and Tx pin
    GPIO_MODER |=  ((GPIO_MODER_OUTPUT << GPIO0_MODER) | (GPIO_MODER_ALTFUNC << GPIO5_MODER) | (GPIO_MODER_ALTFUNC << GPIO6_MODER));  // Set alternate function mode for Rx and Tx pin and output for RS232_EN
    GPIO_AFRL  &= ~((GPIO_AFRL_MASK << GPIO5_AFRL) | (GPIO_AFRL_MASK << GPIO6_AFRL));                                                 // Clear AFRL bits for Tx and Rx pin
    GPIO_AFRL  |=  ((GPIO_AF7 << GPIO5_AFRL) | (GPIO_AF7 << GPIO6_AFRL));                                                             // Set AF7 for Rx and Tx pin
    GPIO_ODR   &= ~(0x01u << RS232_EN);                                                                                               // Clear port pin to power ST232CDR
    //
    // Initialize uart registers
    //
    RCC_APB1ENR |= (1uL << 17);                                      // Enable USART2 clock
    USART_CR2   = 0x200;
    USART_CR1   = 0x2C;
    USART_CR3   = 0x00;
    USART_BRR   = BSP_UART_CLOCK / Baudrate;
    USART_CR1  |= 0x2000;                                            // Enable uart
    //
    // Install USART Handler with preemption level one above lowest level to ensure communication during PendSV
    //
    NVIC_SetPriority(BSP_UART_IRQn, (1u << __NVIC_PRIO_BITS) - 2u);  // Set lowest Priority, ALL BITS set
    NVIC_EnableIRQ(BSP_UART_IRQn);
  }
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
  if (Unit == 0) {
    NVIC_DisableIRQ(BSP_UART_IRQn);
  }
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
  if (Unit == 0) {
    _pfReadCB = pf;
  }
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
  if (Unit == 0) {
    _pfWriteCB = pf;
  }
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
  if (Unit == 0) {
    USART_DR   = Data;
    USART_CR1 |= 0x40;  // Enable tx interrupt
  }
}

/*************************** End of file ****************************/
