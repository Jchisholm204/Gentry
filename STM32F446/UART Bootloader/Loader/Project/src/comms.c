/**
 * @file comms.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2024-04-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "comms.h"
#include "uart.h"

#define PACKET_BUF_LEN (8)


typedef enum comms_state_t{
    CommsState_Length,
    CommsState_Data,
    CommsState_CRC

} comms_state_t;

static comms_state_t state = CommsState_Length;
static uint32_t rx_byte_count;

static comms_packet_t temp_packet = {0};
static comms_packet_t retx_packet = {0};
static comms_packet_t acc_packet = {0};
static comms_packet_t lastTX_packet = {0};

static comms_packet_t packet_buffer[PACKET_BUF_LEN];
static uint32_t packet_read_index = 0, packet_write_index = 0;
static uint32_t packet_buffer_mask = PACKET_BUF_LEN;

static void comms_copy(comms_packet_t *dest, const comms_packet_t *src){
    dest->length = src->length;
    dest->crc = src->length;
    for(uint8_t i = 0; i < PACKET_DATA_LEN; i++){
        dest->data[i] = src->data[i];
    }
}

static bool comms_is(const comms_packet_t *packet, uint8_t is){
    if(packet->length != 1U) return false;
    if(packet->data[0] != is) return false;
    return true;
}

void comms_init(void){
    retx_packet.length = 1;
    retx_packet.data[0] = COMMS_RETX;
    for (uint8_t i= 1; i < PACKET_DATA_LEN; i++) {
        retx_packet.data[i] = 0U;
    }
    retx_packet.crc = comms_compute_crc(&retx_packet);
    acc_packet.length = 1;
    acc_packet.data[0] = COMMS_ACC;
    for (uint8_t i= 1; i < PACKET_DATA_LEN; i++) {
        acc_packet.data[i] = 0U;
    }
    acc_packet.crc = comms_compute_crc(&acc_packet);
}

void comms_tick(void){
    while(uart_read_ready()){
        switch (state){
            case CommsState_Length:
                temp_packet.length = uart_read_byte();
                state = CommsState_Data;
                break;
            case CommsState_Data:
                temp_packet.data[rx_byte_count++] = uart_read_byte();
                if(rx_byte_count >= temp_packet.length){
                    rx_byte_count = 0;
                    state = CommsState_CRC;
                }
                break;
            case CommsState_CRC:
                    temp_packet.crc = uart_read_byte();
                    if(temp_packet.crc != comms_compute_crc(&temp_packet)){
                        comms_write(&retx_packet);
                        state = CommsState_Length;
                        break;
                    }
                    if(comms_is(&temp_packet, COMMS_RETX)){
                        comms_write(&lastTX_packet);
                        state = CommsState_Length;
                    }
                    if(comms_is(&temp_packet, COMMS_ACC)){
                        state = CommsState_Length;
                    }
                    uint32_t next_write_index = (packet_write_index + 1U) & packet_buffer_mask;
                    if(next_write_index == packet_read_index){
                        state = CommsState_Length;
                        break;
                    }
                    comms_copy(&packet_buffer[packet_write_index], &temp_packet);
                    packet_write_index = next_write_index;
                    comms_write(&acc_packet);
                    state = CommsState_Length;
                    break;
            default:
                state = CommsState_Length;
        }
    }
}

bool comms_packets_available(void){
    return !(packet_read_index == packet_write_index);
}

void comms_write(comms_packet_t *packet){
    uart_write((uint8_t*)packet, PACKET_LEN);

}

void comms_read(comms_packet_t *packet){
    comms_copy(packet, &packet_buffer[packet_read_index]);
    packet_read_index = (packet_read_index + 1U) & packet_buffer_mask;
}

uint8_t comms_compute_crc(comms_packet_t *packet){
    return crc8((uint8_t*)&packet, PACKET_LEN-PACKET_CRC_LEN);
}


