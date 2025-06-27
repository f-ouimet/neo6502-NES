#ifndef CPU_UTILS_H_
#define CPU_UTILS_H_
/*
 * Interface with W65C02 CPU
 *
 *  Inspired by and modified from:
 *  2018 Andre Weissflog
 * https://github.com/paulscottrobson/neo6502-firmware/blob/main/firmware/include/system/wdc65C02cpu.h
 */
#include "mem.h"
#include <hardware/gpio.h>
#include <hardware/regs/intctrl.h>
#include <pico/stdio.h>
#include <stdbool.h>

#define CLK_PIN 21
#define OE_MASK 0x700
#define DATA_GPIO 0xFF

void cpu_init() {
  // init address and data bus
  gpio_init_mask(DATA_GPIO);

  // set all outputs to disabled by driving OE pins high
  gpio_init_mask(OE_MASK);
  gpio_set_dir_out_masked(OE_MASK);
  gpio_put_masked(OE_MASK, GPIO_OUT);
  // set clock pin
  gpio_init(CLK_PIN);
  gpio_set_dir(CLK_PIN, 1);
  gpio_put(CLK_PIN, GPIO_OUT);
  // set RW pin
  gpio_init(RW_PIN);
  gpio_set_dir(RW_PIN, GPIO_IN);
}

// set clock low then high
void cpu_clk_tick(uint16_t *addr, bool *rw) {
  gpio_put(CLK_PIN, 0);
  *addr = get_address();
  *rw = gpio_get(RW_PIN);
  gpio_put(CLK_PIN, 1);
}
#endif
