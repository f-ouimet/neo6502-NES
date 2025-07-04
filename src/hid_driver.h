#ifndef HID_DRIVER_H_
#define HID_DRIVER_H_

#include <stdbool.h>
#include <stdint.h>

void keyboard_init(void);

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t idx, const uint8_t *report_desc,
                      uint16_t desc_len);

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t idx);

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t idx,
                                const uint8_t *report, uint16_t len);

#endif
