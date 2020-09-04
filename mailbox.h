#ifndef MAILBOX_H
#define MAILBOX_H

#include <stddef.h> // for size_t
#include <stdint.h> // for uintX_t

#define MB_DEV_SDCARD   0x00000000
#define MB_DEV_UART0    0x00000001
#define MB_DEV_UART1    0x00000002
#define MB_DEV_USBHCD   0x00000003
#define MB_DEV_I2C0     0x00000004
#define MB_DEV_I2C1     0x00000005
#define MB_DEV_I2C2     0x00000006
#define MB_DEV_SPI      0x00000007
#define MB_DEV_CCP2TX   0x00000008

#define MB_MEM_FLAG_DISCARDABLE         1 << 0 // can be resized to 0 at any time, use for cached data
#define MB_MEM_FLAG_DIRECT              1 << 2 // 0xC alias, uncached
#define MB_MEM_FLAG_COHERENT            2 << 2 // 0x8 alias, non-allocating in L2 but coherent
#define MB_MEM_FLAG_L1_NONALLOCATING    ((MB_MEM_FLAG_DIRECT) | (MB_MEM_FLAG_COHERENT))
#define MB_MEM_FLAG_ZERO                1 << 4 // initialise buffer to all zeros
#define MB_MEM_FLAG_NO_INIT             1 << 5 // don't initialise (default is to initialise to all ones)
#define MB_MEM_FLAG_HINT_PERMALOCK      1 << 6 // use if memory is likely to be locked for long periods of time


uint32_t get_firmware_rev();
uint32_t get_board_model();
uint32_t get_board_rev();
uint8_t* get_board_mac(); // returns array with 6 uint8_t entries
uint64_t get_board_serial();

size_t get_arm_mem_size(); // return value in bytes
size_t get_vc_mem_size(); // return value in bytes

uint16_t get_usable_dma_channels(); // bits 0-15: DMA channels 0-15 (0=do not use, 1=usable)

uint32_t get_power_state(uint32_t device_id); // bit 0: 0=off, 1=on; bit 1: 0=device exists, 1=device doesn't exist
uint32_t get_dev_startup_time(uint32_t device_id); // returns time in us for the device's power to become stable after turning it on

uint32_t alloc_vc_mem(size_t size, uint32_t alignment, uint32_t flags); // size & alignment in bytes; returns memory handle
uint32_t lock_vc_mem(uint32_t handle); // returns bus address
uint32_t free_vc_mem(uint32_t handle); // returns status: 0=success

#endif