#include "../include/drivers/vga.h"
#include "../include/drivers/serial.h"
#include "./klog.h"

void kmain()
{
  // Inicializa drivers
  cursor_init();
  serial_init(SERIAL_COM1_BASE);

  klog_ok("CoffeaKernel v0.1.0 initializated!");

  kprint("[OK] ", VGA_COLOR_GREEN, VGA_COLOR_BLACK);
  kprint("CoffeaKernel v0.1.0 initializated!\n", VGA_COLOR_WHITE, VGA_COLOR_BLACK);

  kprint("\n=====================================================\n", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  kprint("\t\t\t\tWelcome to CoffeaOS!\n", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  kprint("=====================================================\n", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  
  while(1) { } // Halt
}
