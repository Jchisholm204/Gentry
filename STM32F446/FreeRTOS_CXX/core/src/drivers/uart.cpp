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

#include "drivers/uart.hpp"

UART::UART(USART_TypeDef *interface, uint32_t baudrate, uint16_t pin_rx, uint16_t pin_tx){
    OK = true;
    // Setup Semaphores
    this->rx_semphr = xSemaphoreCreateBinary();
    if(!this->rx_semphr){
        OK = false;
        return;
    }
    this->tx_semphr = xSemaphoreCreateBinary();
    if(!this->tx_semphr){
        OK = false;
        return;
    }
    
    this->rx_buffer = xStreamBufferCreate(64, 1);
    if(!this->rx_buffer){
        OK = false;
        return;
    }

    hal_uart_init(interface, baudrate, pin_rx, pin_tx);
    xSemaphoreGive(this->rx_semphr);
    xSemaphoreGive(this->tx_semphr);

}
