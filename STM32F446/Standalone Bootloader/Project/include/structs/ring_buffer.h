#pragma once
#define __need_size_t
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    void    *data_buffer;
    uint32_t buffer_len;
    uint32_t data_len;
    uint32_t read_index;
    uint32_t write_index;
} ring_buffer_t;

static inline void ring_buffer_init(ring_buffer_t* rb, void *data_buffer, size_t buffer_size, size_t data_size){
    rb->data_buffer = data_buffer;
    rb->data_len    = data_size;
    rb->buffer_len  = buffer_size;
    rb->read_index  = 0;
    rb->write_index = 0;
}

static inline ring_buffer_t ring_buffer_new(void *data_buffer, size_t buffer_size, size_t data_size){
    return (ring_buffer_t){
        .data_buffer = data_buffer,
        .buffer_len = buffer_size,
        .data_len = data_size,
        .read_index = 0,
        .write_index = 0,
    };
}

static inline bool ring_buffer_write(ring_buffer_t *self, void *data){
    uint32_t new_write_index = (self->write_index + self->data_len) % self->buffer_len;
    if(new_write_index == self->read_index) return false;
    for(uint32_t i = 0; i < self->data_len; i++){
        ((uint8_t*)(self->data_buffer))[i+self->write_index] = ((uint8_t*)data)[i];
    }
    self->write_index = new_write_index;
    return true;
}

static inline bool ring_buffer_read_ready(ring_buffer_t *self){
    return !(self->write_index == self->read_index);
}


static inline bool ring_buffer_read(ring_buffer_t *self, void *data){
    if(!ring_buffer_read_ready(self)) return false;
    for(uint32_t i = 0; i < self->data_len; i++){
        ((uint8_t*)data)[i] = ((uint8_t*)self->data_buffer)[i+self->read_index];
    }
    self->read_index += self->data_len;
    return true;
}

static inline bool ring_buffer_full(ring_buffer_t *self){
    uint32_t new_write_index = (self->write_index + self->data_len) % self->buffer_len;
    return new_write_index == self->read_index;
}


