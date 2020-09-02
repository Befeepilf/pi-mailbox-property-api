#ifndef MAILBOX_H
#define MAILBOX_H

uint32_t get_firmware_rev();
uint32_t get_board_model();
uint32_t get_board_rev();
uint8_t* get_board_mac(); // returns array with 6 uint8_t entries
uint64_t get_board_serial();

#endif