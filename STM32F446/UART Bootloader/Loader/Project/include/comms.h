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

#define PACKET_MAX_LEN 16

// Packet structure
typedef struct comms_packet_t {
    // Valid Packet length
    uint8_t length;
    // Packet data
    uint8_t *data;
    // Packet CRC Checksum
    uint8_t crc;
} comms_packet_t;

// Static packet structure (uses fixed length data array)
typedef struct comms_staticPacket_t {
    // Valid Packet length
    uint8_t length;
    // Packet data
    uint8_t *data;
    // Packet CRC Checksum
    uint8_t crc;
} comms_staticPacket_t;


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

