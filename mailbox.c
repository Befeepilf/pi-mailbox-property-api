#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "mailbox.h"
#include "util.h"


#define MB_TAG_FIRMWARE_REV     0x00000001
#define MB_TAG_BOARD_MODEL      0x00010001
#define MB_TAG_BOARD_REV        0x00010002
#define MB_TAG_BOARD_MAC        0x00010003
#define MB_TAG_BOARD_SERIAL     0x00010004
#define MB_TAG_ARM_MEM          0x00010005
#define MB_TAG_VC_MEM           0x00010006
#define MB_TAG_DMA_CHANNELS     0x00060001
#define MB_TAG_GET_POWER_STATE  0x00020001
#define MB_TAG_DEV_STARTUP_TIME 0x00020002


uint32_t get_firmware_rev()
{
    uint32_t fw_rev = *make_empty_request(MB_TAG_FIRMWARE_REV, 4);
    if (MB_DEBUG)
        printf("Firmware Revision: %u\n", fw_rev);
    return fw_rev;
}

uint32_t get_board_model()
{
    uint32_t model = *make_empty_request(MB_TAG_BOARD_MODEL, 4);
    if (MB_DEBUG)
        printf("Model: %u\n", model);
    return model;
}

uint32_t get_board_rev()
{
    uint32_t rev = *make_empty_request(MB_TAG_BOARD_REV, 4);
    if (MB_DEBUG)
        printf("Revision: %u\n", rev);
    return rev;
}

uint8_t* get_board_mac()
{
    uint8_t* mac = (uint8_t*) make_empty_request(MB_TAG_BOARD_MAC, 6);
    
    if (MB_DEBUG)
    {
        printf("Mac: ");
        for (unsigned int i = 0; i < 6; i++)
        {
            if (i > 0)
                printf(":");
            printf("%02x", mac[i]);
        }
        printf("\n");
    }

    return mac;
}

uint64_t get_board_serial()
{
    uint64_t serial = (uint64_t) *make_empty_request(MB_TAG_BOARD_SERIAL, 8);
    if (MB_DEBUG)
        printf("Serial: %llu\n", serial);
    return serial;
}

size_t get_arm_mem_size()
{
    uint32_t* res = make_empty_request(MB_TAG_ARM_MEM, 8);
    size_t arm_mem_size = res[1];
    if (MB_DEBUG)
        printf("ARM Mem size: %u\n", arm_mem_size);
    return arm_mem_size;
}

size_t get_vc_mem_size()
{
    uint32_t* res = make_empty_request(MB_TAG_VC_MEM, 8);
    size_t vc_mem_size = res[1];
    if (MB_DEBUG)
        printf("VC Mem size: %u\n", vc_mem_size);
    return vc_mem_size;
}

uint16_t get_dma_channels()
{
    uint16_t channels = *make_empty_request(MB_TAG_DMA_CHANNELS, 4);
    if (MB_DEBUG)
    {
        for (unsigned int i = 0; i < 16; i++)
        {
            printf("DMA channel %u: %s\n", i, channels & (1 << i) ? "usable" : "do not use");
        }
    }
    return channels;
}

uint32_t get_power_state(uint32_t device_id)
{
    size_t tag_size;
    uint32_t* tag = create_tag(MB_TAG_GET_POWER_STATE, 4, (uint8_t*) &device_id, 8, &tag_size);
    uint32_t* buff = create_buffer(tag_size, tag);
    uint32_t* res = make_request(buff);

    if (MB_DEBUG)
    {
        printf("Device %u:\n", res[0]);
        printf("\t%s\n", res[1] & 1 ? "on" : "off");
        printf("\t%s\n", res[1] & 1 << 1 ? "does not exist" : "exists");
    }

    return res[1];
}

uint32_t get_dev_startup_time(uint32_t device_id)
{
    size_t tag_size;
    uint32_t* tag = create_tag(MB_TAG_DEV_STARTUP_TIME, 4, (uint8_t*) &device_id, 8, &tag_size);
    uint32_t* buff = create_buffer(tag_size, tag);
    uint32_t* res = make_request(buff);

    if (MB_DEBUG)
    {
        printf("Device %u: %uus\n", res[0], res[1]);
    }

    return res[1];
}