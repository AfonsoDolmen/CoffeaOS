#include "../include/kernel/idt.h"
#include "../include/drivers/vga.h"
#include "klog.h"

static idtr idt_ptr;
static idt_descriptor idt[256];

void define_idt_entry(unsigned char num, unsigned int base, unsigned short selector, unsigned char type_addr)
{
  // Monta endereço do isr
  idt[num].offset_low  = base & 0xFFFF;
  idt[num].offset_high = (base >> 16) & 0xFFFF;

  // Define segmento de código (GDT)
  idt[num].selector = selector;

  idt[num].zero = 0;
  idt[num].type_attr = type_addr;
}

void idt_init()
{
  klog_info("Setting up IDT...");
  vga_set_color(VGA_COLOR_DARKGRAY, VGA_COLOR_BLACK);
  kprint("[INFO] ");
  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  kprint("Setting up IDT...\n");

  // Ponteiro para IDT
  idt_ptr.limit = sizeof(idt) - 1;
  idt_ptr.base = (unsigned int)&idt[0];

  // Interrupções geradas pela CPU
  define_idt_entry(0,  (unsigned int)interrupt_handler_0,  0x08, 0x8E);
  define_idt_entry(6,  (unsigned int)interrupt_handler_6,  0x08, 0x8E);
  define_idt_entry(7,  (unsigned int)interrupt_handler_7,  0x08, 0x8E);
  define_idt_entry(8,  (unsigned int)interrupt_handler_8,  0x08, 0x8E);
  define_idt_entry(10, (unsigned int)interrupt_handler_10, 0x08, 0x8E);
  define_idt_entry(11, (unsigned int)interrupt_handler_11, 0x08, 0x8E);
  define_idt_entry(12, (unsigned int)interrupt_handler_12, 0x08, 0x8E);
  define_idt_entry(13, (unsigned int)interrupt_handler_13, 0x08, 0x8E);
  define_idt_entry(14, (unsigned int)interrupt_handler_14, 0x08, 0x8E);

  // Interrupções de hardware
  define_idt_entry(33, (unsigned int)irq_handler_33, 0x08, 0x8E);

  load_idt(&idt_ptr);
  
  klog_ok("IDT set up");
  vga_set_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
  kprint("[OK]");
  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  kprint("IDT set up\n");
}
