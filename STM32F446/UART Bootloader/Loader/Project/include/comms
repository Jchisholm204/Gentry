/**
 * @file comms.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Packet communication functions for firmware flashing
 * @version 0.1
 * @date 2024-04-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

#define PACKET_DATA_LEN   8U
#define PACKET_LENGTH_LEN 1U
#define PACKET_CRC_LEN    1U
#define PACKET_LEN        (PACKET_DATA_LEN +PACKET_LENGTH_LEN + PACKET_CRC_LEN)

// Retransmit request data[0]
#define COMMS_RETX 0x34
#define COMMS_ACC  0x35

// Packet structure
typedef struct comms_packet_t {
    // Valid Packet length
    uint8_t length;
    // Packet data
    uint8_t data[PACKET_DATA_LEN];
    // Packet CRC Checksum
    uint8_t crc;
} comms_packet_t;


/**
 * @brief Communication initialization function
 * 
 */
void comms_init(void);

/**
 * @brief Communication tick function
 * Run this function in the main loop to handle incoming packets
 */
void comms_tick(void);

/**
 * @brief Check if there are packets available to read
 * 
 * @return true if packets are available
 * @return false if no packets are available
 */
bool comms_packets_available(void);

/**
 * @brief Write a packet to the communication interface
 * 
 * @param packet pointer to the packet to write
 */
void comms_write(comms_packet_t *packet);

/**
 * @brief Read a packet from the communication interface
 * 
 * @param packet pointer to the packet to read into
 */
void comms_read(comms_packet_t *packet);


uint8_t comms_compute_crc(comms_packet_t *packet);


static inline uint8_t crc8(uint8_t *data, uint32_t len){
    uint8_t crc = 0;
    for(uint32_t i = 0; i < len; i++){
        crc ^= data[i];
        for(uint8_t j = 0; j < 8; j++){
            if(crc & 0x80) {
                crc = (uint8_t)(crc << 1U) ^ 0x07U;
            }
            else{
                crc <<= 1U;
            }
        }
    }
    return crc;
}


