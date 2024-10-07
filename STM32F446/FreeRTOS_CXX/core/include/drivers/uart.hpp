/**
 * @file uart.hpp
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief UART C++ Driver
 * @version 0.1
 * @date 2024-10-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "hal/hal_uart.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "stream_buffer.h"



class UART {
    enum eError {eOK, eSemphr, eHW, eUnInit};
    public:
        UART(USART_TypeDef *port_base);
        eError setup(uint32_t baud, uint16_t pin_rx, uint16_t pin_tx);
        eError write_byte(uint8_t data);
        eError write_byte(uint8_t data, TickType_t wait);
        eError write(uint8_t *data, size_t len);
        eError write(uint8_t *data, size_t len, TickType_t wait);
        size_t read(uint8_t *data, size_t len);
        size_t read(uint8_t *data, size_t len, TickType_t wait);
        void isr(void);
    private:
        USART_TypeDef *interface;
        xSemaphoreHandle tx_semphr;
        StreamBufferHandle_t rx_buffer;
        TickType_t def_wait;
        bool OK;
};

extern UART Serial1;
extern UART Serial2;
extern UART Serial3;
extern UART Serial4;
extern UART Serial5;
extern UART Serial6;

extern "C" {
extern void USART1_IRQHandler(void);
extern void USART2_IRQHandler(void);
extern void USART3_IRQHandler(void);
extern void UART4_IRQHandler(void);
extern void UART5_IRQHandler(void);
extern void USART6_IRQHandler(void);
}

