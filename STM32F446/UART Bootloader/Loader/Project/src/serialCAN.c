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
#include "hal_gpio.h"
#include <stddef.h>

#define BUFFER_SIZE 64


static scan_msg_t retx_msg = {0U};
static scan_msg_t acc_msg = {0U};
static bool first_init = true;


void serialCAN_init(SerialCAN_t *self, void (*write_handler)(uint8_t*, size_t), uint8_t (*read_handler)(void), void (*buffer_reset_handler)(void), bool (*read_ready_handler)(void)){
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
    
    self->write_handler = write_handler;
    self->read_handler  = read_handler;
    self->read_ready_handler   = read_ready_handler;
    self->buffer_reset_handler = buffer_reset_handler;

    self->connection_established = false;
    self->rx_failures = 0;
    self->tx_failures = 0;

    self->lastTX_msg.id = 0;
    self->lastTX_msg.len = 0;
    for(int i = 0; i < 8; i++){
        self->lastTX_msg.data[0] = 0;
    }
}

bool serialCAN_connect(SerialCAN_t *self, uint32_t timeout){
    uint8_t buffer[4] = {0};
    if(self->buffer_reset_handler != NULL)
        self->buffer_reset_handler();
    for(uint32_t i = 0; i<timeout; i++){
        if(self->read_ready_handler()){
            buffer[0] = buffer[1];
            buffer[1] = buffer[2];
            buffer[2] = buffer[3];
            buffer[3] = self->read_handler();
        }
        if(*((uint32_t*)buffer) == SERIALCAN_INIT_CODE){
            self->write_handler(buffer, 4);
            self->connection_established = true;
            return true;
        }
        spin(999);
    }
    return false;
}


bool serialCAN_tick(SerialCAN_t *self){
    while(self->read_ready_handler() && self->connection_established){
        uint8_t byte = self->read_handler();
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
                    self->rx_failures++;
                    serialCAN_write(self, &retx_msg);
                }
                else if (self->temp_msg.id == PACKET_ACK_ID && self->temp_msg.len == 0){
                    __asm__("nop");
                }
                else if (self->temp_msg.id == PACKET_RESEND_ID && self->temp_msg.len == 0){
                    if(self->tx_failures > 0) self->tx_failures--;
                    else if(self->rx_failures > 0) self->rx_failures--;
                    else self->connection_established = false;
                    serialCAN_write(self, &self->lastTX_msg);
                }
                else {
                    uint32_t next_write_index = (self->write_index + 1) & (BUFFER_SIZE-1U);
                    if(next_write_index != self->read_index){
                        self->msg_buffer[self->write_index].id = self->temp_msg.id;
                        self->msg_buffer[self->write_index].len = self->temp_msg.len;
                        for(int i = 0; i < 8; i++){
                            self->msg_buffer[self->write_index].data[i] = self->temp_msg.data[i];
                        }
                        self->write_index = next_write_index;
                    }
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
        if(self->rx_failures > 99 || self->tx_failures > 99)
            self->connection_established = false;
    }
    return self->connection_established;
}

void serialCAN_write(SerialCAN_t *self, scan_msg_t * msg){
    // Assume all transmissions are failed until ACK received
    self->tx_failures++;
    self->lastTX_msg.id = msg->id;
    self->lastTX_msg.len = msg->len;
    for(int i =0; i < 8; i++) self->lastTX_msg.data[i] = msg->data[i];
    uint8_t crc = crc8((uint8_t*)msg, PACKET_LEN);
    self->write_handler((uint8_t*)msg, PACKET_LEN);
    self->write_handler(&crc, 1);
}

void serialCAN_read(SerialCAN_t *self, scan_msg_t *msg){
    msg->id = self->msg_buffer[self->read_index].id;
    msg->len = self->msg_buffer[self->read_index].len;
    for(uint8_t i = 0; i < 8; i++){
        msg->data[i] = self->msg_buffer[self->read_index].data[i];
    }
    self->read_index = (self->read_index + 1) & (BUFFER_SIZE-1U);
}

bool serialCAN_read_ready(SerialCAN_t *self){
    return !(self->write_index == self->read_index);
}
