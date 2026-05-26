#include "../include/kernel/isr.h"
#include "../include/drivers/vga.h"
#include "klog.h"

void draw_blue_screen()
{
  vga_set_color(VGA_COLOR_BLUE, VGA_COLOR_BLUE);
  clear();
}

void raise_error_screen(char* msg)
{
  klog_error("Kernel Panic!");
  klog_error(msg);
  
  draw_blue_screen();

  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
  kprint("\t\t\t============== ");

  vga_set_color(VGA_COLOR_RED, VGA_COLOR_BLUE);
  kprint("!!! Kernel Panic !!! ");

  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
  kprint("==============\n");

  // Escreve mensagem de erro
  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
  kprint("\n");
  kprint(msg);
}

void isr_handler(state_registers* state)
{
  switch (state->int_no)
  {
    case 0: raise_error_screen("Division by Zero!"); break;
    default: raise_error_screen("Unknow Exception!"); break;
  }

  // Trava o processamento
  __asm__ volatile("hlt");
}
