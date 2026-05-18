#include "./klog.h"
#include "../include/drivers/serial.h"
#include "../include/drivers/vga.h"

/*
Log por comunicação serial
*/

void klog(char* buff)
{
  serial_write(buff, SERIAL_COM1_BASE);
  serial_write("\n", SERIAL_COM1_BASE);
  kprint(buff, VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  kprint("\n", VGA_COLOR_BLACK, VGA_COLOR_BLACK);
}

void klog_info(char* buff)
{
  serial_write("[INFO] ", SERIAL_COM1_BASE);
  kprint("[INFO]", VGA_COLOR_DARKGRAY, VGA_COLOR_BLACK);
  klog(buff);
}

void klog_ok(char* buff)
{
  serial_write("[OK] ", SERIAL_COM1_BASE);
  kprint("[OK] ", VGA_COLOR_GREEN, VGA_COLOR_BLACK);
  klog(buff);
}

void klog_erro(char* buff)
{
  serial_write("[ERROR] ", SERIAL_COM1_BASE);
  kprint("[ERROR] ", VGA_COLOR_GREEN, VGA_COLOR_BLACK);
  klog(buff);
}
