/*
 * NES memory emulation for the neo6502
 * Inspired by:
 * https://github.com/paulscottrobson/neo6502-firmware/blob/main/firmware/include/system/wdc65C02cpu.h
 */

#include "mem.h"
#include <hardware/gpio.h>
#include <stdint.h>

static uint8_t ram_mem[RAM_MEM_SIZE]; // init memory with size of NES RAM
                                      // (excluding the mirroring)

uint8_t *get_mem() { return ram_mem; }

void mem_init() {

  // set all outputs to disabled by driving OE pins high
  gpio_put(OE1_PIN, 1);
  gpio_put(OE2_PIN, 1);
  gpio_put(OE3_PIN, 1);
}
