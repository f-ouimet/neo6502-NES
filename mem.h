#ifndef MEM_H_
#define MEM_H_

#include <stdint.h>
#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <pico/time.h>

#define RW_PIN 11
#define PHI2_PIN 21

#define ADDR_BASE 0     // A0-A15 on GPIO0-GPIO15
#define DATA_BASE 16    // D0-D7 on GPIO16-GPIO23

#define RAM_MEM_SIZE 2048 //size of RAM

uint8_t *mem_init();

uint8_t *get_mem();

uint16_t read_addr_bus();

uint8_t read_data_bus();

void write_data_bus(uint8_t val);

void release_data_bus();

#endif
