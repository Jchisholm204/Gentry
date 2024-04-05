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

typedef enum comms_state_t{
    CommsState_Length,
    CommsState_Data,
    CommsState_CRC

} comms_state_t;

static comms_state_t state = CommsState_Length;
static uint32_t rx_byte_count;

static comms_staticPacket_t temp_packet = {0};

void comms_init(void){
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

                break;
            default:
                state = CommsState_Length;
        }
    }
}

bool comms_packets_available(void){

}

void comms_write(comms_packet_t *packet){

}

void comms_read(comms_packet_t *packet){

}



