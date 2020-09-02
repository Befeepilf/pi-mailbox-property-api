#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "mailbox.h"
#include "util.h"


#define MB_TAG_FIRMWARE_REV 0x00000001
#define MB_TAG_BOARD_MODEL  0x00010001
#define MB_TAG_BOARD_REV    0x00010002
#define MB_TAG_BOARD_MAC    0x00010003
#define MB_TAG_BOARD_SERIAL 0x00010004
#define MB_TAG_ARM_MEM      0x00010005
#define MB_TAG_VC_MEM       0x00010006


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

int main(int argc, char* argv)
{
    get_board_mac();
    return 0;
}