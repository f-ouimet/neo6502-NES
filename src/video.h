#ifndef VIDEO_H_
#define VIDEO_H_
#include "../include/common_dvi_pin_configs.h"
#include "dvi.h"
#include "dvi_serialiser.h"
#include <hardware/clocks.h>
#include <hardware/irq.h>
#include <hardware/structs/bus_ctrl.h>
#include <hardware/sync.h>
#include <hardware/uart.h>
#include <hardware/vreg.h>
#include <pico/multicore.h>
#include <pico/stdlib.h>
// sizes for video buffers
#define NTSC_SIZE 256 * 224
#define PAL_SIZE 256 * 240

#define NTSC
#if defined(NTSC)
#define WIDTH 256
#define HEIGHT 224
#endif

#if defined(PAL)
#define WIDTH 256
#define HEIGHT 240
#endif

#define VREG_VSEL VREG_VOLTAGE_1_20
#define DVI_TIMING dvi_timing_640x480p_60hz

struct dvi_inst dvi0;

uint frame_ctr = 0;

uint16_t test_video_arr[HEIGHT * WIDTH];
void core1_main() {
  dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);
  while (queue_is_empty(&dvi0.q_colour_valid))
    __wfe();
  dvi_start(&dvi0);
  dvi_scanbuf_main_16bpp(&dvi0);
}

void video_init() {
  vreg_set_voltage(VREG_VSEL);
  sleep_ms(10);
  set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);
  setup_default_uart();

  dvi0.timing = &DVI_TIMING;
  dvi0.ser_cfg = olimex_rp2040_cfg;

  dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());

  // Core 1 will wait until it sees the first colour buffer, then start up the
  // DVI signalling.
  multicore_launch_core1(core1_main);

  // Pass out pointers into our preprepared image, discard the pointers when
  // returned to us. Use frame_ctr to scroll the image

  for (uint y = 0; y < HEIGHT; ++y) {
    for (uint x = 0; x < WIDTH; ++x) {
      // Simple test pattern: vertical gradient from black to red
      uint8_t red = (y * 255) / HEIGHT;
      test_video_arr[y * x] = (red >> 3) << 11; // RGB565: red only
    }
  }
}
void video_task() {

  for (uint y = 0; y < HEIGHT; ++y) {
    uint y_scroll = (y + frame_ctr) % HEIGHT;
    const uint16_t *scanline = &test_video_arr[y_scroll * WIDTH];
    queue_add_blocking_u32(&dvi0.q_colour_valid, &scanline);
    while (queue_try_remove_u32(&dvi0.q_colour_free, &scanline))
      ;
  }
  ++frame_ctr;
}
#endif
