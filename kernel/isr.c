#include "../include/kernel/isr.h"
#include "../include/drivers/vga.h"
#include "klog.h"

void draw_blue_screen()
{
  klog_info("Clearing screen");
  vga_set_color(VGA_COLOR_BLUE, VGA_COLOR_BLUE);
  clear();
}

void isr_handler(state_registers* state)
{
  if (state->int_no == 0)
  {
    draw_blue_screen();

    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    kprint("\t\t\t============== ");

    vga_set_color(VGA_COLOR_RED, VGA_COLOR_BLUE);
    kprint("!!! Kernel Panic !!! ");

    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    kprint("==============\n");
    
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    kprint("\nDivision by Zero!");
    
    while(1)
    {
      __asm__ volatile("hlt");
    }
  }
}
