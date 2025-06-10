/*
 * NES memory emulation for the neo6502
 *
 * Inspired by and modified from:
 *  2018 Andre Weissflog
 * https://github.com/paulscottrobson/neo6502-firmware/blob/main/firmware/include/system/wdc65C02cpu.h
 */

#include "mem.h"
#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <stdint.h>

static uint8_t ram_mem[RAM_MEM_SIZE]; // init memory with size of NES RAM
                                      // (excluding the mirroring)

uint8_t *get_mem() { return ram_mem; }

// Copyright (c) 2018 Andre Weissflog
// Read address bus of w65c02 cpu
// slight modifications were made to the original header file
uint16_t get_address() {
  gpio_set_dir_masked(0xFF, 0);

  gpio_put(OE1_PIN, 0);
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  uint16_t addr = (gpio_get_all() & 0xFF);
  gpio_put(OE1_PIN, 1);

  gpio_put(OE2_PIN, 0);
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  addr |= (gpio_get_all() << (8) & 0xFF00);
  gpio_put(OE2_PIN, 1);

  printf("get addr: %04x\n", addr);

  return addr;
}
// Copyright (c) 2018 Andre Weissflog
// Read data bus of w65c02 cpu
uint8_t get_data() {
  gpio_set_dir_masked(0xFF, 0);

  gpio_put(OE3_PIN, 0);
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  uint8_t data = (gpio_get_all() >> 0) & 0xFF;
  gpio_put(OE3_PIN, 1);

  printf("get data: %02x\n", data);

  return data;
}
// Copyright (c) 2018 Andre Weissflog
// set data bus of w65c02 cpu
void set_data(uint8_t data) {
  gpio_set_dir_masked(0xFF, 0xFF);

  gpio_put_masked(0xFF, data << 0);
  gpio_put(OE3_PIN, 0);
  __asm volatile("nop\n");
  __asm volatile("nop\n");
  gpio_put(OE3_PIN, 1);

  printf("set data: %02x\n", data);
}
