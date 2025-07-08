#include "ff.h"
// then include diskio
#include "diskio.h"
#include <hardware/uart.h>
#include <stdio.h>
#include <tusb.h>

// TODO: PROPER LICENSING

char debug_str[256];
static FATFS fs;
static FILINFO fno;
static FATFS msc_fatfs_volumes[CFG_TUH_DEVICE_MAX];
static volatile bool msc_volume_busy[CFG_TUH_DEVICE_MAX];
static scsi_inquiry_resp_t msc_inquiry_resp;
bool msc_inquiry_complete = false;
void list_files(const char *path);
/*
 *
 * Felix Ouimet
 */

/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

static void wait_for_disk_io(BYTE pdrv) {
  while (msc_volume_busy[pdrv]) {
    tuh_task();
  }
}

static bool disk_io_complete(uint8_t dev_addr,
                             tuh_msc_complete_data_t const *cb_data) {
  (void)cb_data;
  msc_volume_busy[dev_addr] = false;
  return true;
}

DSTATUS disk_status(BYTE pdrv) {
  uint8_t dev_addr = pdrv;
  return tuh_msc_mounted(dev_addr) ? 0 : STA_NODISK;
}

DSTATUS disk_initialize(BYTE pdrv) {
  (void)(pdrv);
  return 0;
}

DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) {
  uint8_t const dev_addr = pdrv;
  uint8_t const lun = 0;
  msc_volume_busy[pdrv] = true;
  tuh_msc_read10(dev_addr, lun, buff, sector, (uint16_t)count, disk_io_complete,
                 0);
  wait_for_disk_io(pdrv);
  return RES_OK;
}

DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count) {
  uint8_t const dev_addr = pdrv;
  uint8_t const lun = 0;
  msc_volume_busy[pdrv] = true;
  tuh_msc_write10(dev_addr, lun, buff, sector, (uint16_t)count,
                  disk_io_complete, 0);
  wait_for_disk_io(pdrv);
  return RES_OK;
}

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff) {
  uint8_t const dev_addr = pdrv;
  uint8_t const lun = 0;
  switch (cmd) {
  case CTRL_SYNC:
    return RES_OK;
  case GET_SECTOR_COUNT:
    *((DWORD *)buff) = (WORD)tuh_msc_get_block_count(dev_addr, lun);
    return RES_OK;
  case GET_SECTOR_SIZE:
    *((WORD *)buff) = (WORD)tuh_msc_get_block_size(dev_addr, lun);
    return RES_OK;
  case GET_BLOCK_SIZE:
    *((DWORD *)buff) = 1; // 1 sector
    return RES_OK;
  default:
    return RES_PARERR;
  }
}
//--------------------------------------------------------------------+
// MACRO TYPEDEF CONSTANT ENUM DECLARATION
//--------------------------------------------------------------------+
static scsi_inquiry_resp_t inquiry_resp;

bool inquiry_complete_cb(uint8_t dev_addr,
                         tuh_msc_complete_data_t const *cb_data) {
  msc_cbw_t const *cbw = cb_data->cbw;
  msc_csw_t const *csw = cb_data->csw;

  if (csw->status != 0) {
    sprintf(debug_str, "Inquiry failed\r\n");
    uart_puts(uart0, debug_str);
    return false;
  }

  // Print out Vendor ID, Product ID and Rev
  sprintf(debug_str, "%.8s %.16s rev %.4s\r\n", inquiry_resp.vendor_id,
          inquiry_resp.product_id, inquiry_resp.product_rev);
  uart_puts(uart0, debug_str);

  // Get capacity of device
  uint32_t const block_count = tuh_msc_get_block_count(dev_addr, cbw->lun);
  uint32_t const block_size = tuh_msc_get_block_size(dev_addr, cbw->lun);

  sprintf(debug_str, "Disk Size: %" PRIu32 " MB\r\n",
          block_count / ((1024 * 1024) / block_size));
  uart_puts(uart0, debug_str);
  sprintf(debug_str, "Block Count = %" PRIu32 ", Block Size: %" PRIu32 "\r\n",
          block_count, block_size);
  uart_puts(uart0, debug_str);
  char drive_path[3] = "0:";
  drive_path[0] += dev_addr;
  FRESULT result = f_mount(&msc_fatfs_volumes[dev_addr], drive_path, 1);
  char s[2];
  if (FR_OK != f_getcwd(s, 2)) {
    f_chdrive(drive_path);
    f_chdir("/");
    list_files("/");
    sprintf(debug_str, "[USB] Reached end of files");
    uart_puts(uart0, debug_str);
  }
  return true;
} //------------- IMPLEMENTATION -------------//
void tuh_msc_mount_cb(uint8_t dev_addr) {
  sprintf(debug_str, "A MassStorage device is mounted\r\n");
  uart_puts(uart0, debug_str);
  uint8_t const lun = 0;
  tuh_msc_inquiry(dev_addr, lun, &inquiry_resp, inquiry_complete_cb, 0);
}

void tuh_msc_umount_cb(uint8_t dev_addr) {
  char drive_path[3] = "0:";
  drive_path[0] += dev_addr;
  f_unmount(drive_path);
  (void)dev_addr;
  sprintf(debug_str, "A MassStorage device is unmounted\r\n");
  uart_puts(uart0, debug_str);
}
//-------------------------------------------//
/************************************
 * Custom funcs here
 ************************************/

// Function to list files recursively
void list_files(const char *path) {
  FRESULT res;
  DIR dir;
  res = f_opendir(&dir, path);
  if (res != FR_OK) {
    sprintf(debug_str, "Failed to open directory: %s\n", path);
    uart_puts(uart0, debug_str);
    return;
  }

  while (1) {
    res = f_readdir(&dir, &fno);
    if (res != FR_OK || fno.fname[0] == 0)
      break;

    if (fno.fattrib & AM_DIR) {
      sprintf(debug_str, "[DIR ] %s/%s\n", path, fno.fname);
      uart_puts(uart0, debug_str);
      char new_path[256];
      snprintf(new_path, sizeof(new_path), "%s/%s", path, fno.fname);
      list_files(new_path); // recurse
    } else {
      sprintf(debug_str, "[FILE] %s/%s\n", path, fno.fname);
      uart_puts(uart0, debug_str);
    }
  }

  f_closedir(&dir);
}
