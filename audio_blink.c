#include "hardware/pwm.h"
#include "pico/stdlib.h"

#include <stdio.h>
#include <stdlib.h>

#define GPIO_ON 1
#define GPIO_OFF 0

#define BUZZ_PIN 20

int main() {
  gpio_init(BUZZ_PIN);
  gpio_set_dir(BUZZ_PIN, GPIO_OUT);

  while (true) {
    gpio_put(BUZZ_PIN, 1);
    sleep_us(500);
    gpio_put(BUZZ_PIN, 0);
    sleep_us(500);
  }
}
