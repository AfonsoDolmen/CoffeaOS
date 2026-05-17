#include "../include/vga.h"

void kmain()
{
  // Inicializa o cursor VGA
  cursor_init();
  
  kprint("Hello From CoffeaOS!\n");
  kprint("CoffeaKernel v0.1.0");

  while(1) { } // Halt
}
