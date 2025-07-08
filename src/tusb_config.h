#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// Required: define MCU and OS
#ifndef CFG_TUSB_MCU
#error CFG_TUSB_MCU must be defined
#endif

#define CFG_TUSB_DEBUG 0

// USB Host enabled
#define CFG_TUH_ENABLED 1

// Use hardware USB host port 0 on RP2040
#define BOARD_TUH_RHPORT 0
#define BOARD_TUH_MAX_SPEED OPT_MODE_DEFAULT_SPEED
#define CFG_TUH_MAX_SPEED BOARD_TUH_MAX_SPEED

// Memory alignment for DMA
#define CFG_TUH_MEM_ALIGN __attribute__((aligned(4)))

// Buffer for device enumeration
#define CFG_TUH_ENUMERATION_BUFSIZE 256

// Host class drivers enabled
#define CFG_TUH_HUB 2
#define CFG_TUH_MSC 1
#define CFG_TUH_HID 2
#define CFG_TUH_CDC 0
#define CFG_TUH_VENDOR 0

// Support up to 3 devices behind 1 hub
#define CFG_TUH_DEVICE_MAX 5 // 3 devices + 1 hub

// Mass Storage config
#define CFG_TUH_MSC_MAXLUN 4

// HID: supports multiple interfaces (in our case: keyboard + controller)
#define CFG_TUH_HID_ITF_NUM 4

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */
