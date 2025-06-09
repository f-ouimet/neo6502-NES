#ifndef MEM_H_
#define MEM_H_

#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <pico/time.h>
#include <stdint.h>

#define OE1_PIN 8
#define OE2_PIN 9
#define OE3_PIN 10

#define RW_PIN 11
#define PHI2_PIN 21

#define MEM_BASE 0 // GPIO 0 to 7 for memory and address access

#define RAM_MEM_SIZE 2048 // size of NES RAM (excluding mirroring)

void mem_init();

uint8_t *get_mem();

uint16_t read_addr_bus();

uint8_t read_data_bus();

void write_data_bus(uint8_t val);

void release_data_bus();

#endif
