#include "drivers/uart.h"
#include "hal/hal_uart.h"
#include "pins.h"
#include "nvicConfig.h"


UART_t Serial2;


void uart_init(uint32_t baud){
    Serial2.USART = USART2;
    hal_uart_init(USART2, baud, PIN_USART2_RX, PIN_USART2_TX);
    ring_buffer_init(&Serial2.rx_rb, Serial2.rx_buffer, UART_BUFFER_SIZE, sizeof(uint8_t));
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(USART2_IRQn, NVIC_Priority_MIN);
    hal_uart_enable_rxne(USART2, true);
}

bool uart_write(UART_t *self, uint8_t *buf, size_t len){
    hal_uart_write_buf(self->USART, (char*)buf, len);
    return true;
}

bool uart_write_byte(UART_t *self, uint8_t byte){
    hal_uart_write_byte(self->USART, byte);
    return true;
}

bool uart_read_ready(UART_t *self){
    return ring_buffer_read_ready(&self->rx_rb);
}

size_t uart_read(UART_t *self, uint8_t *buf, size_t len){
    for(size_t i = 0; i < len; i++){
        if(!ring_buffer_read_ready(&self->rx_rb)) return i;
        ring_buffer_read(&self->rx_rb, &buf[i]);
    }
    return len;
}

uint8_t uart_read_byte(UART_t *self){
    uint8_t byte;
    ring_buffer_read(&self->rx_rb, &byte);
    return byte;
}

uint8_t rXbyte = 0;

void USART2_IRQHandler(void){
    if(hal_uart_read_ready(USART2)){
        uint8_t byte = hal_uart_read_byte(USART2);
        rXbyte = byte;
        // hal_uart_write_byte(USART2, byte);
        ring_buffer_write(&Serial2.rx_rb, &byte);
    }
    return;
}

