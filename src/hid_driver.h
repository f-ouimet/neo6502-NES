#include <hardware/uart.h>
#include <stdio.h>
#include <tusb.h>

char string[100];

void keyboard_init(void) {
  // setup if needed
}

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t idx, const uint8_t *report_desc,
                      uint16_t desc_len) {

  sprintf(string, "HID mounted: addr=%d instance=%d\n", dev_addr, idx);
  uart_puts(uart0, string);
  tuh_hid_receive_report(dev_addr, idx);
}

// called when a report is received from the HID device
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance,
                                uint8_t const *report, uint16_t len) {
  const uint8_t modifier = report[0];
  for (int i = 2; i < 8; i++) {
    uint8_t keycode = report[i];
    if (keycode) {
      sprintf(string, "[HID] Keycode: 0x%02X (modifier: 0x%02X)\n", keycode,
              modifier);
      uart_puts(uart0, string);
    }
  }

  // Continue receiving
  tuh_hid_receive_report(dev_addr, instance);
}
// Called when a HID device is unmounted
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
  sprintf(string, "HID unmounted: addr=%d instance=%d\n", dev_addr, instance);
  uart_puts(uart0, string);
}
