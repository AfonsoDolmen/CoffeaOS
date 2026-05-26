#include "../include/drivers/vga.h"
#include "../include/drivers/serial.h"
#include "../include/kernel/gdt.h"
#include "../include/kernel/idt.h"
#include "./klog.h"

void kmain()
{
  // Inicializa drivers
  vga_init();
  serial_init(SERIAL_COM1_BASE);

  // Inicializa GDT e IDT
  gdt_init();
  idt_init();

  klog_ok("CoffeaKernel v0.1.0 initializated!");

  vga_set_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
  kprint("[OK] ");
  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  kprint("CoffeaKernel v0.1.0 initializated!\n");

  kprint("\n=====================================================\n");
  kprint("\t\t\t\tWelcome to CoffeaOS!\n");
  kprint("=====================================================\n");

  int result = 10 / 0;
  
  while(1) { } // Halt
}
