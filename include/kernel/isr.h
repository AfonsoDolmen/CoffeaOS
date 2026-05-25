#ifndef ISR_H
#define ISR_H

// Estado dos registradores antes da interrupção
typedef struct {
  unsigned int edi;
  unsigned int esi;
  unsigned int ebp;
  unsigned int esp;
  unsigned int ebx;
  unsigned int edx;
  unsigned int ecx;
  unsigned int eax;

  unsigned int int_no;     // Número da interrupçao
  unsigned int error_code; // Código de erro é gerado somente por interrupções da CPU

  unsigned int eip;
  unsigned int cs;
  unsigned int eflags;
}__attribute__((packed)) state_registers;

extern void isr_handler(state_registers* state);

#endif
