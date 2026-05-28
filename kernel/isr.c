#include "../include/kernel/isr.h"
#include "../include/drivers/vga.h"
#include "../include/drivers/pic.h"
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

  // Verifica se há error code
  if (state->error_code != 0)
  {
    kprintf("\nError code: %x", state->error_code);
  }

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

  // Trava o processamento
  asm volatile("hlt");
}

void isr_handler(state_registers* state)
{
  // Exceções geradas pela CPU
  if (state->int_no < 32)
  {
    switch (state->int_no)
    {
      case  0: raise_error_screen("Division by Zero!", state); break;
      case  6: raise_error_screen("Invalid Opcode!", state); break;
      case  7: raise_error_screen("Device not Available!", state); break;
      case  8: raise_error_screen("Double Fault!", state); break;
      case 10: raise_error_screen("Invalid TSS!", state); break;
      case 11: raise_error_screen("Segment not Present!", state); break;
      case 12: raise_error_screen("Stack-Segment Fault!", state); break;
      case 13: raise_error_screen("General Protection Fault!", state); break;
      case 14: raise_error_screen("Page Fault!", state); break;
      default: raise_error_screen("Unknow Exception!", state); break;
    }
  }

  // Interrupções de hardware
  if (state->int_no >= 32)
  {
    switch(state->int_no)
    {
      // TODO: Driver teclado
      case 33: klog_info("Key pressed!"); pic_eoi(state->int_no - 32); break;
    }
  }
}
