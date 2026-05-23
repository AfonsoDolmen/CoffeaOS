#include "../include/kernel/gdt.h"
#include "klog.h"

// Ponteiro para a GDT
static gdtr gdt_ptr;

// Cria o array de descritores (3 descritores contando com nulo)
static gdt_descriptor gdt[DESCRIPTOR_ARRAY_SIZE];

void descriptor_define_bits(gdt_descriptor* gdt, unsigned int base, unsigned int limit, unsigned char access, unsigned char flags)
{
  // Base
  gdt->base_low    = (base & 0xFFFF);
  gdt->base_middle = (base >> 16) & 0xFF;
  gdt->base_high   = (base >> 24) & 0xFF;

  // Limite
  gdt->limit_low = (limit & 0xFFFF);
  gdt->flags     = ((limit >> 16) & 0x0F) | (flags & 0xF0);

  // Acesso
  gdt->access = access;
}

void gdt_init()
{
  // Log
  klog_info("Setting up GDT...");
  
  // Aponta para a estrutra GDT
  gdt_ptr.limit = (sizeof(gdt_descriptor) * DESCRIPTOR_ARRAY_SIZE) - 1;
  gdt_ptr.base  = (unsigned int)&gdt[0]; // Base começa no descritor nulo

  // Define os bits para o descritor de segmento nulo, código e dados
  descriptor_define_bits(&gdt[0], 0, 0, 0, 0); // Descritor nulo, por padrão é tudo 0

  // Segmento Kernel Code e Kernel Data
  klog_info("GDT: Setting Kernel Code and Kernel Data...");
  descriptor_define_bits(&gdt[1], 0, 0xFFFFFFFF, 0x9A, 0xCF);
  descriptor_define_bits(&gdt[2], 0, 0xFFFFFFFF, 0x92, 0xCF);
  klog_ok("GDT: Kernel Code and Kernel Data set up ");


  // Segmento User Code e User Data
  klog_info("GDT: Setting User Code and User Data...");
  descriptor_define_bits(&gdt[3], 0, 0xFFFFFFFF, 0xFA, 0xCF);
  descriptor_define_bits(&gdt[4], 0, 0xFFFFFFFF, 0xF2, 0xCF);
  klog_ok("GDT: User Code And User Data set up");

  load_gdt(&gdt_ptr);
}
