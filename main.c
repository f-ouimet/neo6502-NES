
#include "cpu_utils.h"
#include "mem.h"
#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <pico/time.h>

#define GPIO_ON 1
#define GPIO_OFF 0

#define BUZZ_PIN 20

int main() {
  stdio_init_all();
  cpu_init();
  gpio_init(BUZZ_PIN);
  gpio_set_dir(BUZZ_PIN, GPIO_OUT);

  while (true) {
    // test square wave to see if program halts
    gpio_put(BUZZ_PIN, 1);
    sleep_us(500);
    gpio_put(BUZZ_PIN, 0);
    sleep_us(500);
  }
}
