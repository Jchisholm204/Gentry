/**
 * @file serial.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief UART Serial Driver
 * @version 0.1
 * @date 2024-10-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _SERIAL_H_
#define _SERIAL_H_
#include "os/hal/hal_uart.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "stream_buffer.h"
#include "os/config/sys_cfg.h"

enum eSerialError {
    eSerialOK,
    // Hardware Error
    eSerialHW,
    // Semaphore Lock Timeout
    eSerialSemphr,
    // Initialization Failed
    eSerialInitFail,
    // The Serial interface has not been Initialized
    eSerialNoInit,
    // Null reference passed
    eSerialNULL
};

typedef enum eSerialError eSerialError;

typedef struct Serial {
    USART_TypeDef *UART;
    SemaphoreHandle_t semphr_hndl;
    StaticSemaphore_t static_semphr;
    StreamBufferHandle_t rx_buf;
    eSerialError state;
} Serial_t;


/**
 * @brief Initialize a Serial Interface
 *
 * @param pHndl Pointer to Serial Handle
 * @param dev UART device to Initialize
 * @param pin_rx RX Pin
 * @param pin_tx TX Pin
 * @returns eSerialOK or Error
 */
extern eSerialError serial_init(Serial_t *pHndl, unsigned long baud, pin_t pin_rx, pin_t pin_tx);

/**
 * @brief Write to a Serial Interface (BLOCKING)
 *
 * @param pHndl Handle to write to
 * @param buf buffer to write
 * @param len length of buffer (in bytes)
 * @param timeout timeout to wait for semaphore
 * @returns eSerialOK or Error
 */
extern eSerialError serial_write(Serial_t *pHndl, char *buf, size_t len, TickType_t timeout);

/**
 * @brief Sttach an RX buffer to a Serial Interface
 *
 * @param pHndl Serial Interface to attach the RX buffer to
 * @param pBuf Pointer to the Serial Buffer Structure
 * @returns eSerialOK or Error
 */
extern eSerialError serial_attach(Serial_t *pHndl, StreamBufferHandle_t buf_hndl);

/**
 * @brief detach an RX buffer to a Serial Interface
 *
 * @param pHndl Serial Interface to detach from
 * @returns eSerialOK or Error
 */
extern eSerialError serial_detach(Serial_t *pHndl);


#if (configUSE_SERIAL1 == 1)
extern void USART1_IRQHandler(void);
extern Serial_t Serial1;
#endif
#if (configUSE_SERIAL2 == 1)
extern void USART2_IRQHandler(void);
extern Serial_t Serial2;
#endif
#if (configUSE_SERIAL3 == 1)
extern void USART3_IRQHandler(void);
extern Serial_t Serial3;
#endif
#if (configUSE_SERIAL4 == 1)
extern void UART4_IRQHandler(void);
extern Serial_t Serial4;
#endif
#if (configUSE_SERIAL5 == 1)
extern void UART5_IRQHandler(void);
extern Serial_t Serial5;
#endif
#if (configUSE_SERIAL6 == 1)
extern void USART6_IRQHandler(void);
extern Serial_t Serial6;
#endif

#endif

