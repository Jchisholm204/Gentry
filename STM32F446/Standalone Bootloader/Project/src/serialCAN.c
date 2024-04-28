/**
 * @file serialCAN.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief CAN bus packet emulation over UART (Serial) Comms
 * @version 1.1
 * @date 2024-04-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "serialCAN.h"
#include <stddef.h>

#define BUFFER_SIZE 64


static SerialCAN_msg_t retx_msg = {0U};
static SerialCAN_msg_t acc_msg = {0U};
static bool first_init = true;


void serialCAN_init(SerialCAN_t *self, UART_t *pSerial){
    if(first_init){
        retx_msg.id = PACKET_RESEND_ID;
        retx_msg.len = 0;
        acc_msg.id = PACKET_ACK_ID;
        acc_msg.len = 0;
        for(int i = 0; i < 8; i++){
            retx_msg.data[i] = 0;
            acc_msg.data[i] = 0;
        }
        first_init = false;
    }
    
    self->state = STATE_ID;
    ring_buffer_init(&self->rx_rb, self->msg_buffer, SERIALCAN_BUFFER_SIZE, sizeof(SerialCAN_msg_t));
    self->pSerial = pSerial;

    self->lastTX_msg.id = 0;
    self->lastTX_msg.len = 0;
    for(int i = 0; i < 8; i++){
        self->lastTX_msg.data[0] = 0;
    }

    self->rx_byte_count = 0;
    self->ok = true;
    self->tx_failures = 0;
}


bool serialCAN_tick(SerialCAN_t *self){
    self->ok = !(self->tx_failures > 99);
    // if(!uart_read_ready(self->pSerial)) return self->ok;
    // uint8_t buf[BUFFER_SIZE];
    // size_t len = uart_read(self->pSerial, buf, BUFFER_SIZE);
    // if(len == 0) return self->ok;
    // for(size_t i = 0; i < len; i++){
    while(uart_read_ready(self->pSerial)){
        uint8_t byte;
        uart_read(self->pSerial, &byte, 1);
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
                    // serialCAN_write(self, &retx_msg);
                }
                else if (self->temp_msg.id == PACKET_ACK_ID && self->temp_msg.len == 0){
                    if(self->tx_failures > 0) self->tx_failures--;
                }
                else if (self->temp_msg.id == PACKET_RESEND_ID && self->temp_msg.len == 0){
                    // serialCAN_write(self, &self->lastTX_msg);
                }
                else {
                    // ring_buffer_write(&self->rx_rb, &self->temp_msg);
                    serialCAN_write(self, &acc_msg);
                }
                self->temp_msg.id = 0;
                self->temp_msg.len = 0;
                for (int j = 0; j < 8; j++){
                    self->temp_msg.data[j] = 0;
                }
                self->state = STATE_ID;
                break;
            }
        }
    }
    return self->ok;
}

void serialCAN_write(SerialCAN_t *self, SerialCAN_msg_t * msg){
    // Assume all transmissions are failed until ACK received
    if(msg->id != PACKET_ACK_ID || msg->id != PACKET_RESEND_ID){
        self->tx_failures++;
        self->lastTX_msg.id = msg->id;
        self->lastTX_msg.len = msg->len;
        for(int i =0; i < 8; i++) self->lastTX_msg.data[i] = msg->data[i];
    }
    uint8_t crc = crc8((uint8_t*)msg, PACKET_LEN);
    uart_write(self->pSerial, (uint8_t*)msg, PACKET_LEN);
    uart_write_byte(self->pSerial, crc);
}

bool serialCAN_read(SerialCAN_t *self, SerialCAN_msg_t *msg){
    if(!ring_buffer_read_ready(&self->rx_rb)) return false;
    ring_buffer_read(&self->rx_rb, msg);
    return true;
}

bool serialCAN_read_ready(SerialCAN_t *self){
    return ring_buffer_read_ready(&self->rx_rb);
}
