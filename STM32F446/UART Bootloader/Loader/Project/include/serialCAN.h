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

#define PACKET_LEN 13
typedef struct {
    uint32_t id;
    uint8_t len;
    uint8_t data[8];
} scan_msg_t;

#ifndef PACKET_RESEND_ID
    #define PACKET_RESEND_ID 404UL
#endif

#ifndef PACKET_ACK_ID
    #define PACKET_ACK_ID 80UL
#endif


void serialCAN_init(void);

void serialCAN_tick(void);

void serialCAN_write(scan_msg_t * msg);

void serialCAN_read(scan_msg_t *msg);

bool serialCAN_read_ready(void);


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



