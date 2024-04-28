/**
 * @file serialCAN.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief CAN bus packet emulation over UART (Serial) Comms
 * @version 1.1
 * @date 2024-04-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "drivers/uart.h"
#include "structs/ring_buffer.h"

#define PACKET_LEN 13
typedef struct {
    uint32_t id;
    uint8_t len;
    uint8_t data[8];
} SerialCAN_msg_t;

#ifndef PACKET_RESEND_ID
    #define PACKET_RESEND_ID 404UL
#endif

#ifndef PACKET_ACK_ID
    #define PACKET_ACK_ID 80UL
#endif

#ifdef BUFFER_SIZE
    #define SERIALCAN_BUFFER_SIZE BUFFER_SIZE
#else
    #define SERIALCAN_BUFFER_SIZE 64
#endif

typedef struct {
    enum STATE {
        STATE_ID,
        STATE_LEN,
        STATE_DATA,
        STATE_CRC
    } state;
    SerialCAN_msg_t msg_buffer[SERIALCAN_BUFFER_SIZE];
    ring_buffer_t rx_rb;
    UART_t *pSerial;
    SerialCAN_msg_t temp_msg;
    SerialCAN_msg_t lastTX_msg;
    uint8_t rx_byte_count;
    uint8_t tx_failures;
    bool ok;
} SerialCAN_t;

void serialCAN_init(SerialCAN_t *self, UART_t *pSerial);

bool serialCAN_tick(SerialCAN_t *self);

void serialCAN_write(SerialCAN_t *self, SerialCAN_msg_t * msg);

bool serialCAN_read(SerialCAN_t *self, SerialCAN_msg_t * msg);

bool serialCAN_read_ready(SerialCAN_t *self);


static inline uint8_t crc8(uint8_t *data, uint32_t len){
    uint8_t crc = 0;
    for(uint32_t i = 0; i < len; i++){
        crc ^= data[i];
        for(uint8_t j = 0; j < 8; j++){
            if(crc & 0x80) {
                crc = (uint8_t)(crc << 1U) ^ 0x07U;
            }
            else{
                crc = (uint8_t)(crc << 1);
            }
        }
    }
    return crc;
}



