
#include "cpu_utils.h"
// INCLUDE FIRST
#include "ff.h"
// INCLUDE DISKIO AFTER
#include "diskio.h"

#include "hid_driver.h"
#include "mem.h"
#include "msc_driver.h"
#include "tusb_config.h"
#include "video.h"
#include <hardware/gpio.h>
#include <hardware/structs/io_bank0.h>
#include <hardware/uart.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <tusb.h>

#define GPIO_ON 1
#define GPIO_OFF 0
#define UART_TX_PIN 28
#define UART_RX_PIN 29

#define BUZZ_PIN 20

char buffer[100];

int main() {

  video_init();

  uart_init(uart0, 9600);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
  uart_puts(uart0, "UART_INIT_OK \n");

  tuh_init(BOARD_TUH_RHPORT);

  cpu_init();
  gpio_init(BUZZ_PIN);
  gpio_set_dir(BUZZ_PIN, GPIO_OUT);

  uint8_t *ram = get_mem();
  if (!ram) {
    uart_puts(uart0, "Mem alloc failed\n");
    return 1;
  }
  // preload in ram for test
  ram[0x8000] = 0xA9;
  ram[0x8001] = 0x42;
  ram[0x8002] = 0x8D;
  ram[0x8003] = 0x00;
  ram[0x8004] = 0x20;
  // reset vector
  ram[0xFFFC] = 0x00;
  ram[0xFFFD] = 0x80;
  static uint16_t addr;
  static bool rw;
  static uint8_t data;

  while (true) {

    // tinyusb will call the appropriate task depending on events
    tuh_task();

    // uart_putc(uart0, 'c'); //clock test
    //
    //  test square wave to see if program halts
    gpio_put(BUZZ_PIN, 1);
    sleep_us(1000);
    gpio_put(BUZZ_PIN, 0);
    sleep_us(1000);
    gpio_put(BUZZ_PIN, 1);
    sleep_us(1000);
    gpio_put(BUZZ_PIN, 0);
    sleep_us(1000);

    // cpu tick
    cpu_clk_tick(&addr, &rw);
    // emulate mem instructions
    if (rw) {
      set_data(ram[addr]);
    } else {
      data = get_data();
      ram[addr] = data;
    }

    // test prints
    // sprintf(buffer, "test init! \n");
    // uart_puts(uart0, buffer);
    // sprintf(buffer, "add 2000: %x\n", ram[0x2000]);
    // uart_puts(uart0, buffer);
    // sprintf(buffer, "rw: %d\n", rw);
    // uart_puts(uart0, buffer);
    //

    video_task();
  }
}
