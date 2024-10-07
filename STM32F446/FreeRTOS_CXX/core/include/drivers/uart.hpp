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
    public:
        UART(USART_TypeDef *interface, uint32_t baudrate, uint16_t pin_rx, uint16_t pin_tx);
        void write(uint8_t data);
        void write_buf(uint8_t *data, size_t len);
        uint8_t read();
        void read_buf(uint8_t *data, size_t len);
    private:
        USART_TypeDef *interface;
        uint32_t baudrate;
        xSemaphoreHandle rx_semphr, tx_semphr;
        StreamBufferHandle_t rx_buffer;
        bool OK;
};
