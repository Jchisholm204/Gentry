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

  Device : STM32F401
  Board  : ST STM32F401C-Discovery

  Unit | UART   | Board connector
  ===============================
  0    | USART6 | Tx=PC6, Rx=PC7

*/

#include "BSP_UART.h"
#include "RTOS.h"        // For OS_INT_Enter()/OS_INT_Leave(). Remove this line and OS_INT_* functions if not using OS.
#include "stm32f4xx.h"   // Device specific header file, contains CMSIS defines.

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define BSP_UART_CLOCK        (SystemCoreClock)
#define BSP_UART_IRQn         (USART6_IRQn)
#define BSP_UART_IRQHandler   (USART6_IRQHandler)

#define USART_BASE_ADDR       (0x40011400u)  // USART6
#define USART_SR              (*(volatile unsigned long*)(USART_BASE_ADDR + 0x00u))
#define USART_DR              (*(volatile unsigned long*)(USART_BASE_ADDR + 0x04u))
#define USART_BRR             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x08u))
#define USART_CR1             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x0Cu))

#define RCC_BASE_ADDR         (0x40023800u)
#define RCC_APB1ENR           (*(volatile unsigned long*)(RCC_BASE_ADDR + 0x40u))
#define RCC_AHB1ENR           (*(volatile unsigned long*)(RCC_BASE_ADDR + 0x30u))
#define RCC_APB2ENR           (*(volatile unsigned long*)(RCC_BASE_ADDR + 0x44u))

#define GPIO_BASE_ADDR        (0x40020800u) // GPIOC
#define GPIO_MODER            (*(volatile unsigned long*)(GPIO_BASE_ADDR + 0x00u))
#define GPIO_OTYPER           (*(volatile unsigned long*)(GPIO_BASE_ADDR + 0x04u))
#define GPIO_OSPEEDR          (*(volatile unsigned long*)(GPIO_BASE_ADDR + 0x08u))
#define GPIO_PUPDR            (*(volatile unsigned long*)(GPIO_BASE_ADDR + 0x0Cu))
#define GPIO_AF_LOW           (*(volatile unsigned long*)(GPIO_BASE_ADDR + 0x20u))

#define US_RXRDY              (0x20u)   // RXNE
#define US_TXEMPTY            (0x80u)   // TXE
#define USART_RX_ERROR_FLAGS  (0x0Fu)   // ORE/NE/FE/PE

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
  //
  // Unused parameters. Partially hard coded USART settings.
  //
  BSP_UART_USE_PARA(NumDataBits);
  BSP_UART_USE_PARA(Parity);
  BSP_UART_USE_PARA(NumStopBits);
  if (Unit == 0) {
    //
    // Setup clocks, GPIO ports and NVIC IRQs.
    //
    RCC_AHB1ENR  |= (1uL <<  2);    // GPIO CLK enable
    RCC_APB2ENR  |= (1uL <<  5);    // Enable USART6 clock
                                    // GPIOC set alternate function for USART6
    GPIO_AF_LOW   = (8uL << 24)     // - Set pin_6 to AF7
                  | (8uL << 28);    // - Set pin_7 to AF7
                                    // GPIOC alternate function mode
    GPIO_MODER    = (2uL << 12)     // - Pin_6 AF
                  | (2uL << 14);    // - Pin_7 AF
                                    // GPIOC speed setting
    GPIO_OSPEEDR  = (2uL << 12)     // - Pin_6 fast speed
                  | (2uL << 14);    // - Pin_7 fast speed
    GPIO_OTYPER   = 0;              // Output type: push-pull for pin_6 and pin_7
                                    // Pull-up/pull-down register
    GPIO_PUPDR    = (1uL << 12)     // - Pin_6 pull-up
                  | (1uL << 14);    // - Pin_7 pull-up
    //
    // Initialize IRQ.
    //
    NVIC_SetPriority(BSP_UART_IRQn, (1u << __NVIC_PRIO_BITS) - 2u);
    NVIC_EnableIRQ(BSP_UART_IRQn);
    //
    // Initialize USART.
    //
    _SetBaudrate(Unit, Baudrate);   // Set baudrate
    USART_CR1 = (1uL <<  3)         // Transmitter enable
              | (1uL <<  2)         // Receiver enable
              | (1uL <<  5)         // RX interrupt enable
              | (1uL << 13);        // Enable USART
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
    USART_CR1 = 0x00000000;
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
    USART_DR   = Data;  // Send data.
    USART_CR1 |= 0x40;  // Enable Tx interrupt.
  }
}

/*************************** End of file ****************************/
