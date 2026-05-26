#include "../include/kernel/isr.h"
#include "../include/drivers/vga.h"
#include "klog.h"

void draw_blue_screen()
{
  vga_set_color(VGA_COLOR_BLUE, VGA_COLOR_BLUE);
  clear();
}

void raise_error_screen(char* msg, state_registers* state)
{
  klog_error("Kernel Panic!");
  klog_error(msg);

  // Desenha tela azul
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

  // Número da exceção
  kprintf("\nException Number: %d", (signed int)state->int_no);

  // Escreve estado dos registradores
  kprintf("\n\nEAX: %x,", state->eax);
  kprintf(" EBX: %x,", state->ebx);
  kprintf(" ECX: %x,", state->ecx);
  kprintf(" EDX: %x", state->edx);

  kprintf("\n\nCS: %x,", state->cs);
  kprintf(" EIP: %x", state->eip);

  // Informações da pilha
  kprintf("\n\nESP: %x,", state->esp);
  kprintf(" EBP: %x", state->ebp);
}

void isr_handler(state_registers* state)
{
  switch (state->int_no)
  {
    case 0: raise_error_screen("Division by Zero!", state); break;
    default: raise_error_screen("Unknow Exception!", state); break;
  }

  // Trava o processamento
  __asm__ volatile("hlt");
}
