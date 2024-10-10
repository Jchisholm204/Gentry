/**
 * @file serial.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief UART Serial Driver
 * @version 0.1
 * @date 2024-10-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "os/drivers/serial.h"


void generic_handler(Serial_t *pHndl){
    // MUST read input port to clear iPending bit
    uint8_t rx_data = hal_uart_read_byte(pHndl->UART);
    // hal_uart_write_byte(pHndl->UART, rx_data);
    // Check that a handler exists
    if(pHndl->rx_buf == NULL || pHndl->state != eSerialOK){
        // If there is no handler disable this interrupt
        hal_uart_enable_rxne(pHndl->UART, false);
        return;
    }
    // If a buffer exists, run the interrupt routine
    BaseType_t higher_woken = pdFALSE;
    xStreamBufferSendFromISR(*pHndl->rx_buf, &rx_data, sizeof(rx_data), &higher_woken);
    portYIELD_FROM_ISR(higher_woken);
}

#if (configUSE_SERIAL1 == 1)
void USART1_IRQHandler(void){
    generic_handler(&Serial1)
}
Serial_t Serial1 = {
    USART1,
    NULL,
    {0},
    NULL,
    eSerialNoInit
};
#endif
#if (configUSE_SERIAL2 == 1)
void USART2_IRQHandler(void){
    generic_handler(&Serial2);
}
Serial_t Serial2 = {
    USART2,
    NULL,
    {0},
    NULL,
    eSerialNoInit
};
#endif
#if (configUSE_SERIAL3 == 1)
void USART3_IRQHandler(void){
    generic_handler(&Serial3)
}
Serial_t Serial3 = {
    USART3,
    NULL,
    {0},
    NULL,
    eSerialNoInit
};
#endif
#if (configUSE_SERIAL4 == 1)
void UART4_IRQHandler(void){
    generic_handler(&Serial4)
}
Serial_t Serial4 = {
    UART4,
    NULL,
    {0},
    NULL,
    eSerialNoInit
};
#endif
#if (configUSE_SERIAL5 == 1)
void UART5_IRQHandler(void){
    generic_handler(&Serial5)
}
Serial_t Serial5 = {
    UART5,
    NULL,
    {0},
    NULL,
    eSerialNoInit
};
#endif
#if (configUSE_SERIAL6 == 1)
void USART6_IRQHandler(void){
    generic_handler(&Serial6)
}
Serial_t Serial6 = {
    USART6,
    NULL,
    {0},
    NULL,
    eSerialNoInit
};
#endif


eSerialError serial_init(Serial_t *pHndl, unsigned long baud, pin_t pin_rx, pin_t pin_tx){
    if(!pHndl)
        return eSerialNULL;
    if(pHndl->state == eSerialOK)
        return pHndl->state;
    hal_uart_init(pHndl->UART, baud, pin_tx, pin_rx);
    pHndl->semphr_hndl = xSemaphoreCreateBinaryStatic(&pHndl->static_semphr);
    if(pHndl->semphr_hndl == NULL)
        return eSerialInitFail;
    xSemaphoreGive(pHndl->semphr_hndl);
    pHndl->state = eSerialOK;
    return pHndl->state;
}

eSerialError serial_write(Serial_t *pHndl, char *buf, size_t len, TickType_t timeout){
    if(pHndl == NULL || buf == NULL)
        return eSerialNULL;
    if(pHndl->state != eSerialOK)
        return pHndl->state;
    if(xSemaphoreTake(pHndl->semphr_hndl, timeout) == pdTRUE){
        hal_uart_write_buf(pHndl->UART, buf, len);
        xSemaphoreGive(pHndl->semphr_hndl);
        return eSerialOK;
    }
    return eSerialSemphr;
}

eSerialError serial_attach(Serial_t *pHndl, StreamBufferHandle_t *buf_hndl){
    if(pHndl == NULL || buf_hndl == NULL)
        return eSerialNULL;
    if(pHndl->state != eSerialOK)
        return pHndl->state;
    if(pHndl->rx_buf == NULL){
        pHndl->rx_buf = buf_hndl;
        hal_uart_enable_rxne(pHndl->UART, true);
        return eSerialOK;
    }
    return eSerialSemphr;
}


