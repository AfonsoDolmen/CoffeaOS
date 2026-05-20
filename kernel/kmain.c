#include "../include/drivers/vga.h"
#include "../include/drivers/serial.h"
#include "./klog.h"

void kmain()
{
  // Inicializa drivers
  cursor_init();
  serial_init(SERIAL_COM1_BASE);

  klog_ok("CoffeaKernel v0.1.0 Initializated!");
  kprint("Welcome to CoffeaOS!\n", VGA_COLOR_GREEN, VGA_COLOR_BLACK);
  
    
  for (unsigned int i = 0; i < FB_COL_HEIGHT; i++)
  {
    kprint("Scroll\n", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  }
  
  while(1) { } // Halt
}
