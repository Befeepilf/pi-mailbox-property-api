#ifndef MAILBOX_UTIL_H
#define MAILBOX_UTIL_H

#include <stdint.h>

// returns vcio file descriptor
int open_vcio();

/*
    rounds size UP such that it is a multiple of block
    size & block are in bytes
*/
size_t align_to_block(size_t size, size_t block);

/*
    value_size & tag_size are in bytes
    tag_size will be set by this function
*/
uint32_t* create_tag(uint32_t id, size_t value_size, uint8_t* value, size_t* tag_size);

// value_size is in bytes
uint32_t* create_buffer(size_t value_size, uint32_t* value);

// returns size in bytes of response made by VC
size_t make_request(uint32_t* buffer);

/*
    use this function for requests including a single tag with an empty request value
    res_size is the expected response size in bytes
*/
uint32_t* make_empty_request(uint32_t tag_id, size_t res_size);

#endif