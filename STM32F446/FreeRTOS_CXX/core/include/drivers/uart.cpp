/**
 * @file uart.cpp
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief UART C++ Driver
 * @version 0.1
 * @date 2024-10-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

// #include "hal"

class UART {
    public:
        UART(USART_TypeDef *port, uint32_t baudrate);
        void write(uint8_t data);
        void write_buf(uint8_t *data, size_t len);
        uint8_t read();
        void read_buf(uint8_t *data, size_t len);
    private:
        USART_TypeDef *port;
        uint32_t baudrate;
};