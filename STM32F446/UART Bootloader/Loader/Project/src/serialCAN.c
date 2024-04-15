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
#include "ringBuffer.h"
#include "uart.h"

#define BUFFER_SIZE 64

typedef enum {
    STATE_ID,
    STATE_LEN,
    STATE_DATA,
    STATE_CRC
} serialCAN_state_t;

static serialCAN_state_t state = STATE_ID;
static can_msg_t data_buffer[BUFFER_SIZE];
static uint32_t rx_byte_count = 0;
static can_msg_t temp_msg = {0U};
static can_msg_t retx_msg = {0U};
static can_msg_t acc_msg = {0U};
static can_msg_t lastTX_msg = {0U};


void serialCAN_init(void){
}

void serialCAN_tick(void){}

void serialCAN_write(can_msg_t * msg){
    (void)msg;
}

void serialCAN_read(can_msg_t *msg){
    (void)msg;
}

bool serialCAN_read_ready(void){
    return false;
}
