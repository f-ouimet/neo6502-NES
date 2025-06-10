#ifndef MEM_H_
#define MEM_H_

#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <pico/time.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define OE1_PIN 8
#define OE2_PIN 9
#define OE3_PIN 10

#define RW_PIN 11

#define MEM_BASE 0 // GPIO 0 to 7 for memory and address access

#define RAM_MEM_SIZE                                                           \
  65536 // size of NES addressable memory (excluding mirroring)

uint8_t *get_mem();

uint16_t get_address();

uint8_t get_data();

void set_data(uint8_t data);

#endif
