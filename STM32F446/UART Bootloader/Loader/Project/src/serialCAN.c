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

void serialCan_init(void){}

void serialCan_tick(void){}

void serialCan_write(can_msg_t * msg){
    (void)msg;
}

void serialCan_read(can_msg_t *msg){
    (void)msg;
}

bool serialCan_read_ready(void){
    return false;
}
