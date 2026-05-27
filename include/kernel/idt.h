#ifndef IDT_H
#define IDT_H

// Estrutura que aponta para a base da IDT
typedef struct {
  unsigned short limit; // Tamanho da IDT
  unsigned int base;  // Endereço base da IDT
}__attribute__((packed)) idtr;

// Define a estrutura de um descritor da IDT
/*
offset_low  -> Parte menos significativa do endereço que contém o código do handler
selector    -> Segmento de código da GDT (0x08 - flush_registers)
zero        -> Byte reservado
type_addr   -> flags (present, DPL, S, type)
offset_high -> Parte mais significativa do endereço que contém o código do handler
*/
typedef struct {
  unsigned short offset_low;
  unsigned short selector;
  unsigned char  zero;
  unsigned char  type_attr;
  unsigned short offset_high;
  
}__attribute__((packed)) idt_descriptor;

// Funções Assembly
extern void load_idt(idtr* idt_base);
extern void interrupt_handler_0();

extern void irq_handler_33();

// void define_idt_entry(unsigned char num, unsigned int base, unsigned short selector, unsigned char type_addr);
void idt_init();

#endif
