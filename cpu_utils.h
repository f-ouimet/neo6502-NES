#ifndef CPU_UTILS_H_
#define CPU_UTILS_H_
/*
 * Interface with W65C02 CPU
 *
 *  Inspired by:
 * https://github.com/paulscottrobson/neo6502-firmware/blob/main/firmware/include/system/wdc65C02cpu.h
 */
#include "mem.h"
#include <hardware/gpio.h>
#include <hardware/regs/intctrl.h>
#include <stdbool.h>

#define RESET_PIN 26
#define IRQ_PIN 28
#define OE_MASK 0x700
#define DATA_BUS_MASK 0xFF

void cpu_init() {
  // init address and data bus
  gpio_init_mask(DATA_BUS_MASK);

  // set all outputs to disabled by driving OE pins high
  gpio_init_mask(OE_MASK);
  gpio_set_dir_out_masked(OE_MASK);
  gpio_put_masked(OE_MASK, GPIO_OUT);
  // set clock pin
  gpio_init(PHI2_PIN);
  gpio_set_dir(PHI2_PIN, 1);
  gpio_put(PHI2_PIN, GPIO_OUT);
  // set RW pin
  gpio_init(RW_PIN);
  gpio_set_dir(RW_PIN, GPIO_IN);

  // set IRQ_pin
  gpio_init(IRQ_PIN);
  gpio_set_dir(IRQ_PIN, GPIO_OUT);
  gpio_put(IRQ_PIN, 1);
  // set reset pin
  gpio_init(RESET_PIN);
  gpio_set_dir(RESET_PIN, GPIO_OUT);
}

// set clock low then high
void cpu_clk_tick() {}
#endif
