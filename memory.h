#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include <stdlib.h>

#define RW_PIN 11
#define PHI2_PIN 21

#define ADDR_BASE 0     // A0-A15 on GPIO0-GPIO15
#define DATA_BASE 16    // D0-D7 on GPIO16-GPIO23

#define MEM_SIZE 2048

uint8_t *get_mem();

void mem_init(uint8_t *mem);

uint16_t read_addr_bus(uint8_t *mem);

uint8_t read_data_bus(uint8_t *mem);

void write_data_bus(uint8_t *mem);

void release_data_bus(uint8_t *mem);
