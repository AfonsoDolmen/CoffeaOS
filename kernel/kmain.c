#include "../include/drivers/vga.h"
#include "../include/drivers/serial.h"
#include "../include/drivers/pic.h"
#include "../include/drivers/keyboard.h"
#include "../include/drivers/ata.h"
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

  // Inicia PIC
  pic_init();

  // ATA driver
  ata_init();

  // Ativa teclado
  keyboard_init();

  // Ativa as interrupções
  asm volatile("sti");

  klog_ok("CoffeaKernel v0.1.0 initializated!");

  vga_set_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
  kprint("[OK] ");
  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  kprint("CoffeaKernel v0.1.0 initializated!\n");

  kprint("\n=====================================================\n");
  kprint("\t\t\t\tWelcome to CoffeaOS!\n");
  kprint("=====================================================\n");

  // Cria buffer para leitura de setor
  uint16_t buffer[256];

  // Lê setor 1
  ata_lba28_read_sectors(1, 1, buffer);

  // Cast
  char* disk_data = (char*)buffer;
  kprintf("%s\n", disk_data);

  while(1) { } // Halt
}
