
/**
 * @file ringBuffer.h
 * @author Jacob Chisholm (https://jchisholm.github.io)
 * @brief 
 * @date 2024-04-01
 * 
 */

#pragma once
#include <stm32f4xx.h>
#include <stdbool.h>

typedef struct ring_buffer_t {
    uint8_t *buffer;
    uint32_t mask;
    uint32_t read_index;
    uint32_t write_index;
} ring_buffer_t;


void ring_buffer_init(ring_buffer_t *rb, uint8_t *buffer, uint32_t size);

bool ring_buffer_empty(ring_buffer_t *rb);

bool ring_buffer_write(ring_buffer_t *rb, uint8_t byte);

bool ring_buffer_read(ring_buffer_t *rb, uint8_t *byte);
