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
#include "uart.h"
#include "pins.h"
#include "hal_gpio.h"

#define BUFFER_SIZE 64

typedef enum {
    STATE_ID,
    STATE_LEN,
    STATE_DATA,
    STATE_CRC
} serialCAN_state_t;

static serialCAN_state_t state = STATE_ID;
static scan_msg_t data_buffer[BUFFER_SIZE];
static uint32_t write_index = 0, read_index = 0;
static uint32_t rx_byte_count = 0;
static scan_msg_t temp_msg = {0U};
static scan_msg_t retx_msg = {0U};
static scan_msg_t acc_msg = {0U};
static scan_msg_t lastTX_msg = {0U};

void serialCAN_init(void){
    state = STATE_ID;
    retx_msg.id = PACKET_RESEND_ID;
    retx_msg.len = 0;
    acc_msg.id = PACKET_ACK_ID;
    acc_msg.len = 0;
    lastTX_msg.id = 0;
    lastTX_msg.len = 0;
    for(int i = 0; i < 8; i++){
        retx_msg.data[i] = 0;
        acc_msg.data[i] = 0;
        lastTX_msg.data[0] = 0;
    }
}

void serialCAN_tick(void){
    while(uart_read_ready()){
        uint8_t byte = uart_read_byte();
        switch (state) {
            case STATE_ID: {
                temp_msg.id |= (uint32_t)((uint32_t)byte << (8*rx_byte_count));
                rx_byte_count++;
                if(rx_byte_count >= 4) {
                    rx_byte_count = 0;
                    state = STATE_LEN;
                }
                break;
            }
            case STATE_LEN: {
                temp_msg.len = byte;
                temp_msg.len = temp_msg.len > 8 ? 8 : temp_msg.len;
                rx_byte_count = 0;
                state = STATE_DATA;
                break;
            }
            case STATE_DATA: {
                temp_msg.data[rx_byte_count] = byte;
                rx_byte_count++;
                if(rx_byte_count >= 8){
                    rx_byte_count = 0;
                    state = STATE_CRC;
                }
                break;
            }
            case STATE_CRC: {
                uint8_t temp_crc = byte;
                uint8_t crc = crc8((uint8_t*)&temp_msg, PACKET_LEN);
                if(crc != temp_crc){
                    serialCAN_write(&retx_msg);
                    // gpio_write(debug_led2, !gpio_read_odr(debug_led2));
                }
                else if (temp_msg.id == PACKET_ACK_ID && temp_msg.len == 0){
                    __asm__("nop");
                }
                else if (temp_msg.id == PACKET_RESEND_ID && temp_msg.len == 0){
                    //uart_write((uint8_t*)&lastTX_msg, sizeof(scan_msg_t));
                    serialCAN_write(&lastTX_msg);
                }
                else {
                    uint32_t next_write_index = (write_index + 1) & (BUFFER_SIZE-1U);
                    if(next_write_index != read_index){
                        data_buffer[write_index].id = temp_msg.id;
                        data_buffer[write_index].len = temp_msg.len;
                        for(int i = 0; i < 8; i++){
                            data_buffer[write_index].data[i] = temp_msg.data[i];
                        }
                        write_index = next_write_index;
                    }
                    serialCAN_write(&acc_msg);
                }
                temp_msg.id = 0;
                temp_msg.len = 0;
                for (int i = 0; i < 8; i++){
                    temp_msg.data[i] = 0;
                }
                state = STATE_ID;
                break;
            }
        }
    }
}

void serialCAN_write(scan_msg_t * msg){
    gpio_write(debug_led1, true);
    lastTX_msg.id = msg->id;
    lastTX_msg.len = msg->len;
    for(int i =0; i < 8; i++) lastTX_msg.data[i] = msg->data[i];
    uint8_t crc = crc8((uint8_t*)msg, PACKET_LEN);
    uart_write((uint8_t*)msg, PACKET_LEN);
    uart_write_byte(crc);
    gpio_write(debug_led1, false);
}

void serialCAN_read(scan_msg_t *msg){
    msg->id = data_buffer[read_index].id;
    msg->len = data_buffer[read_index].len;
    for(uint8_t i = 0; i < 8; i++){
        msg->data[i] = data_buffer[read_index].data[i];
    }
    read_index = (read_index + 1) & (BUFFER_SIZE-1U);
}

bool serialCAN_read_ready(void){
    return !(write_index == read_index);
}
