#include "mem.h"


static uint8_t ram_mem[RAM_MEM_SIZE]; //init memory with size of NES RAM (excluding the mirroring)

uint8_t *get_mem(){
    return ram_mem;
}

uint8_t *mem_init(){
    for (int i = 0; i < 16; i++) {
        gpio_init(ADDR_BASE + i);
        gpio_set_dir(ADDR_BASE + i, GPIO_IN);
    }
    gpio_init(PHI2_PIN);
    gpio_set_dir(PHI2_PIN, GPIO_IN);

    gpio_init(RW_PIN);
    gpio_set_dir(RW_PIN, GPIO_IN);

    return ram_mem;
}

uint16_t read_addr_bus(){
    uint16_t addr = 0u;
    for (int i = 0; i < 16; i++) {
        addr |= gpio_get(ADDR_BASE + i) << i;
    }
    return addr;
}

uint8_t read_data_bus(){
    uint8_t data = 0u;
    for (int i = 0; i < 8; i++) {
        data |= gpio_get(DATA_BASE + i) << i;
    }
    return data;
}

void write_data_bus(uint8_t val){
    for (int i = 0; i < 8; i++) {
        gpio_set_dir(DATA_BASE + i, GPIO_OUT);
        gpio_put(DATA_BASE + i, (val >> i) & 1);
    }
}

void release_data_bus(){
     for (int i = 0; i < 8; i++) {
        gpio_set_dir(DATA_BASE + i, GPIO_IN);
    }
}