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
#include "os/config/nvic.h"

UART::UART(USART_TypeDef *port_base){
    // interface has not been initialized
    this->OK = false;
    this->interface = port_base;
    // Default waiting period is 100 ticks
    this->def_wait = 100;
}


UART::eError UART::setup(uint32_t baud, uint16_t pin_rx, uint16_t pin_tx){
    if(this->OK)
        return eOK;

    // Setup Semaphores
    this->tx_semphr = xSemaphoreCreateBinaryStatic(&this->tx_sSemphr);
    this->rx_semphr = xSemaphoreCreateBinaryStatic(&this->rx_sSemphr);

    // Setup the UART interface
    hal_uart_init(this->interface, baud, pin_rx, pin_tx);
    
    
    // Release the semaphores to be used
    xSemaphoreGive(this->tx_semphr);
    xSemaphoreGive(this->rx_semphr);
    OK = true;
    // Return Success
    return eOK;
}

UART::eError UART::write_byte(uint8_t data){
    if(!this->OK)
        return eUnInit;
    if(xSemaphoreTake(this->tx_semphr, this->def_wait) == pdTRUE){
        hal_uart_write_byte(this->interface, data);
        xSemaphoreGive(this->tx_semphr);
        return eOK;
    }
    return eSemphr;
}

UART::eError UART::write_byte(uint8_t data, TickType_t wait){
    if(!this->OK)
        return eUnInit;
    if(xSemaphoreTake(this->tx_semphr, wait) == pdTRUE){
        hal_uart_write_byte(this->interface, data);
        xSemaphoreGive(this->tx_semphr);
        return eOK;
    }
    return eSemphr;
}

UART::eError UART::write(uint8_t *data, size_t len){
    if(!this->OK)
        return eUnInit;
    if(xSemaphoreTake(this->tx_semphr, this->def_wait) == pdTRUE){
        hal_uart_write_buf(this->interface, (char*)data, len);
        xSemaphoreGive(this->tx_semphr);
        return eOK;
    }
    return eSemphr;
}

UART::eError UART::write(uint8_t *data, size_t len, TickType_t wait){
    if(!this->OK)
        return eUnInit;
    if(xSemaphoreTake(this->tx_semphr, wait) == pdTRUE){
        hal_uart_write_buf(this->interface, (char*)data, len);
        xSemaphoreGive(this->tx_semphr);
        return eOK;
    }
    return eSemphr;
}

UART::eError UART::attach(StreamBufferHandle_t hndl){
    if(!hndl)
        return eNoMem;
    if(xSemaphoreTake(this->rx_semphr, this->def_wait) == pdTRUE){
        eError hw_err = this->enable_interrupt();
        if(hw_err != eOK){
            xSemaphoreGive(this->rx_semphr);
            return hw_err;
        }
        this->rx_buffer = hndl;
        return eOK;
    }
    return eSemphr;
}

UART::eError UART::detatch(void){
    vPortEnterCritical();
    xSemaphoreGive(this->rx_semphr);
    return this->disable_interrupt();
    vPortExitCritical();
}

UART::eError UART::enable_interrupt(void){
    // Enable the UART interrupt
    hal_uart_enable_rxne(this->interface, true);

    switch((int)this->interface){
        case (int)USART1_BASE:
            NVIC_SetPriority(USART1_IRQn, NVIC_Priority_MIN);
            NVIC_EnableIRQ(USART1_IRQn);
            break;
        case (int)USART2_BASE:
            NVIC_SetPriority(USART2_IRQn, NVIC_Priority_MIN);
            NVIC_EnableIRQ(USART2_IRQn);
            break;
        case (int)USART3_BASE:
            NVIC_SetPriority(USART3_IRQn, NVIC_Priority_MIN);
            NVIC_EnableIRQ(USART3_IRQn);
            break;
        case (int)UART4_BASE:
            NVIC_SetPriority(UART4_IRQn, NVIC_Priority_MIN);
            NVIC_EnableIRQ(UART4_IRQn);
            break;
        case (int)UART5_BASE:
            NVIC_SetPriority(UART5_IRQn, NVIC_Priority_MIN);
            NVIC_EnableIRQ(UART5_IRQn);
            break;
        case (int)USART6_BASE:
            NVIC_SetPriority(USART6_IRQn, NVIC_Priority_MIN);
            NVIC_EnableIRQ(USART6_IRQn);
            break;
        default:
            return eHW;
    }
    return eOK;
}

UART::eError UART::disable_interrupt(void){
    // Enable the UART interrupt
    hal_uart_enable_rxne(this->interface, false);

    switch((int)this->interface){
        case (int)USART1_BASE:
            NVIC_DisableIRQ(USART1_IRQn);
            break;
        case (int)USART2_BASE:
            NVIC_DisableIRQ(USART2_IRQn);
            break;
        case (int)USART3_BASE:
            NVIC_DisableIRQ(USART3_IRQn);
            break;
        case (int)UART4_BASE:
            NVIC_DisableIRQ(UART4_IRQn);
            break;
        case (int)UART5_BASE:
            NVIC_DisableIRQ(UART5_IRQn);
            break;
        case (int)USART6_BASE:
            NVIC_DisableIRQ(USART6_IRQn);
            break;
        default:
            return eHW;
    }
    return eOK;
}


void UART::isr(void){
    if(!xPortIsInsideInterrupt())
        return;
    if(!this->eOK)
        return;
    BaseType_t higher_woken = pdFALSE;
    uint8_t rx_data = hal_uart_read_byte(this->interface);
    xStreamBufferSendFromISR(this->rx_buffer, &rx_data, sizeof(rx_data), &higher_woken);
    portYIELD_FROM_ISR(higher_woken);
}

