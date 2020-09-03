#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "debug.h"
#include "util.h"

int open_vcio()
{
    int vcio_fd;
    if ((vcio_fd = open("/dev/vcio", O_RDWR|O_SYNC)) < 0)
    {
        printf("Cannot open /dev/vcio: %s\n", strerror(errno));
        exit(-1);
    }

    return vcio_fd;
}

size_t align_to_block(size_t size, size_t block)
{
    return ((size - 1) / block + 1) * block;
}

uint32_t* create_tag(uint32_t id, size_t value_size, uint8_t* value, size_t res_size, size_t* tag_size)
{
    assert(value_size > 0 ? value != NULL : value == NULL);
    assert(tag_size != NULL);

    // align tag to 4 bytes
    size_t aligned_value_size = align_to_block(res_size > value_size ? res_size : value_size, 4);
    *tag_size = 3 * sizeof(uint32_t) + aligned_value_size;
    uint32_t* tag = malloc(*tag_size);

    unsigned int i = 0;
    tag[i++] = id;
    tag[i++] = aligned_value_size;
    tag[i++] = 0; // 0x0 indicates request

    uint8_t* tag_val = (uint8_t*) &tag[i];
    for (unsigned int j = 0; j < value_size; j++)
    {
        tag_val[j] = value[j];
    }

    // padding for response buffer & 4 bytes alignment
    for (int j = 0; j < aligned_value_size - value_size; j++)
    {
        tag_val[value_size + j] += 0;
    }

    return tag;
}

uint32_t* create_buffer(size_t value_size, uint32_t* value)
{
    assert(value_size > 0 ? value != NULL : value == NULL);

    size_t buffer_size = 3 * sizeof(uint32_t) + value_size;
    uint32_t* buffer = malloc(buffer_size);

    unsigned int i = 0;
    buffer[i++] = buffer_size;
    buffer[i++] = 0x00000000;

    uint8_t* buff_val = (uint8_t*) &buffer[i];
    for (unsigned int j = 0; j < value_size; j++)
    {
        buff_val[j] = ((uint8_t*) value)[j];
    }

    // uint32_t end tag (4 * uint8_t)
    for (unsigned int j = 0; j < 4; j++)
    {
        buff_val[value_size + j] = 0x0;
    }

    return buffer;
}

uint32_t* make_request(uint32_t* buffer)
{
    int vcio_fd = open_vcio();

    if (MB_DEBUG)
    {
        printf("buff size: %u\n", buffer[0]);
        printf("buff req code: %u\n", buffer[1]);
        printf("tag id: %u\n", buffer[2]);
        printf("tag val size: %u\n", buffer[3]);
        printf("tag req code: %u\n", buffer[4]);
    }

    if (ioctl(vcio_fd, _IOWR(100, 0, char*), buffer) < 0)
    {
        printf("ioctl: request failed: %s\n", strerror(errno));
        exit(-1);
    }

    close(vcio_fd);

    uint32_t res_code = buffer[1];
    if (res_code == 0x80000000)
    {
        if (MB_DEBUG)
            printf("ioctl: request was successful\n");
    }
    else if (res_code == 0x80000001)
    {
        printf("ioctl: error parsing request buffer\n");
        exit(-1);
    }
    else
    {
        printf("ioctl: unknown response code\n");
        exit(-1);
    }

    uint32_t tag_res_code = buffer[4];

    if (tag_res_code >> 31 == 0x0)
    {
        printf("mailbox: unknown request tag\n");
        exit(-1);
    }

    uint32_t tag_res_len = tag_res_code ^ 0x80000000; // ignore most significant bit
    
    if (MB_DEBUG)
        printf("Response size: %u bytes\n", tag_res_len);

    return &buffer[5];
}

uint32_t* make_empty_request(uint32_t tag_id, size_t res_size)
{
    size_t tag_size;

    uint32_t* tag = create_tag(
        tag_id,
        0,
        NULL,
        res_size,
        &tag_size
    );

    uint32_t* buffer = create_buffer(tag_size, tag);
    uint32_t* res = make_request(buffer);

    return res;
}