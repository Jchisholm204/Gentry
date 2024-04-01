
#include "ringBuffer.h"

void ring_buffer_init(ring_buffer_t *rb, uint8_t *buffer, uint32_t size){
    rb->buffer = buffer;
    rb->read_index = 0;
    rb->write_index = 0;
    rb->mask = size - 1;
}

bool ring_buffer_empty(ring_buffer_t *rb){
    return (rb->read_index == rb->write_index);
}

bool ring_buffer_write(ring_buffer_t *rb, uint8_t byte){
    uint32_t local_read = rb->read_index;
    uint32_t local_write = rb->write_index;
    
    uint32_t next_write = (local_write + 1U) & rb->mask;

    if(next_write == local_read) return false;
    
    rb->buffer[local_write] = byte;
    rb->write_index = next_write;
    return true;

}

bool ring_buffer_read(ring_buffer_t *rb, uint8_t *byte){
    uint32_t local_read = rb->read_index;
    uint32_t local_write = rb->write_index;
    if(local_read == local_write) return false;
    
    *byte = rb->buffer[local_read];
    local_read = (uint32_t)((local_read + 1U) & rb->mask);
    rb->read_index = local_read;

    return true;
}

