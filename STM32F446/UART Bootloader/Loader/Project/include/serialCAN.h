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


void serialCan_init(void);

void serialCan_tick(void);

void serialCan_write(can_msg_t * msg);

void serialCan_read(can_msg_t *msg);

bool serialCan_read_ready(void);
