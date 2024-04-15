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

#ifndef can_msg_t

typedef struct {
    uint32_t id;
    uint8_t data[8];
    uint8_t len;
    uint8_t format;
    uint8_t type;
    uint32_t timestamp;
} can_msg_t;

#endif


#ifndef CAN_FORMAT
/* Symbolic names for formats of CAN message                                 */
typedef enum {STANDARD_FORMAT = 0, EXTENDED_FORMAT} CAN_FORMAT;

#endif

#ifndef CAN_FRAME
/* Symbolic names for type of CAN message                                    */
typedef enum {DATA_FRAME = 0, REMOTE_FRAME}         CAN_FRAME;

#endif

#ifndef PACKET_RESEND_ID

#define PACKET_RESEND_ID 404UL

#endif

#ifndef PACKET_ACK_ID

#define PACKET_ACK_ID 80

#endif


void serialCAN_init(void);

void serialCAN_tick(void);

void serialCAN_write(can_msg_t * msg);

void serialCAN_read(can_msg_t *msg);

bool serialCAN_read_ready(void);
