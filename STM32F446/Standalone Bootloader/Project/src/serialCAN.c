/**
 * @file serialCAN.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief CAN bus packet emulation over UART (Serial) Comms
 * @version 1.3
 * @date 2024-04-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#define BUFFER_SIZE 64
#include "serialCAN.h"
#include "drivers/uart.h"


static SerialCAN_msg_t retx_msg = {0U};
static SerialCAN_msg_t acc_msg = {0U};

void serialCAN_init(SerialCAN_t *self, UART_t *pSerial){
    self->state = STATE_ID;
    retx_msg.id = PACKET_RESEND_ID;
    retx_msg.len = 0;
    acc_msg.id = PACKET_ACK_ID;
    acc_msg.len = 0;
    self->lastTX_msg.id = 0;
    self->lastTX_msg.len = 0;
    for(int i = 0; i < 8; i++){
        retx_msg.data[i] = 0;
        acc_msg.data[i] = 0;
        self->lastTX_msg.data[0] = 0;
    }
    self->pSerial = pSerial;
}

bool serialCAN_tick(SerialCAN_t *self){
    while(uart_read_ready(self->pSerial)){
        uint8_t byte = uart_read_byte(self->pSerial);
        switch (self->state) {
            case STATE_ID: {
                self->temp_msg.id |= (uint32_t)((uint32_t)byte << (8*self->rx_byte_count));
                self->rx_byte_count++;
                if(self->rx_byte_count >= 4) {
                    self->rx_byte_count = 0;
                    self->state = STATE_LEN;
                }
                break;
            }
            case STATE_LEN: {
                self->temp_msg.len = byte;
                self->temp_msg.len = self->temp_msg.len > 8 ? 8 : self->temp_msg.len;
                self->rx_byte_count = 0;
                self->state = STATE_DATA;
                break;
            }
            case STATE_DATA: {
                self->temp_msg.data[self->rx_byte_count] = byte;
                self->rx_byte_count++;
                if(self->rx_byte_count >= 8){
                    self->rx_byte_count = 0;
                    self->state = STATE_CRC;
                }
                break;
            }
            case STATE_CRC: {
                uint8_t temp_crc = byte;
                uint8_t crc = crc8((uint8_t*)&self->temp_msg, PACKET_LEN);
                if(crc != temp_crc){
                    serialCAN_write(self, &retx_msg);
                    // gpio_write(debug_led2, !gpio_read_odr(debug_led2));
                }
                else if (self->temp_msg.id == PACKET_ACK_ID && self->temp_msg.len == 0){
                    __asm__("nop");
                }
                else if (self->temp_msg.id == PACKET_RESEND_ID && self->temp_msg.len == 0){
                    //uart_write((uint8_t*)&lastTX_msg, sizeof(scan_msg_t));
                    serialCAN_write(self, &self->lastTX_msg);
                }
                else {
                    ring_buffer_write(&self->rx_rb, &self->temp_msg);
                    serialCAN_write(self, &acc_msg);
                }
                self->temp_msg.id = 0;
                self->temp_msg.len = 0;
                for (int i = 0; i < 8; i++){
                    self->temp_msg.data[i] = 0;
                }
                self->state = STATE_ID;
                break;
            }
        }
    }
    return true;
}

void serialCAN_write(SerialCAN_t *self, SerialCAN_msg_t * msg){
    uint8_t crc = crc8((uint8_t*)msg, PACKET_LEN);
    uart_write(self->pSerial, (uint8_t*)msg, PACKET_LEN);
    uart_write_byte(self->pSerial, crc);
}

bool serialCAN_read(SerialCAN_t *self, SerialCAN_msg_t *msg){
    return ring_buffer_read(&self->rx_rb, msg);
}

bool serialCAN_read_ready(SerialCAN_t *self){
    return ring_buffer_read_ready(&self->rx_rb);
}
