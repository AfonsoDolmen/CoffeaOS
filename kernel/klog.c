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
}

void klog_info(char* buff)
{
  serial_write("[INFO] ", SERIAL_COM1_BASE);
  klog(buff);
}

void klog_ok(char* buff)
{
  serial_write("[OK] ", SERIAL_COM1_BASE);
  klog(buff);
}

void klog_erro(char* buff)
{
  serial_write("[ERROR] ", SERIAL_COM1_BASE);
  klog(buff);
}
