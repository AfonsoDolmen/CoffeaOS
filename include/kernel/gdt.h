#ifndef GDT_H
#define GDT_H

// Layout de memória que aponta para a GDT
typedef struct {
  unsigned short limit; // Tamanho do array GDT em bytes (gdt_length - 1)
  unsigned int base;    // Endereço base da GDT
} __attribute__((packed)) gdtr;

// Estrutura de um descritor da GDT
/*
base        -> Endereço base do segmento (base:limit): 32 bits
limit       -> Tamanho total do segmento: 20 bits
access byte -> Define se o segmento deve ser usado para escrita/leitura/execução: 8 bits
flags       -> Caracteristicas do segmento (granulrity, size, long mode e AVL): 4 bits

Verificar capitulo sobre segmentos manual Intel 
*/
typedef struct {
  unsigned short limit_low;   // Primeiros 16 bits de limite (limit_high = 4 bits menos significativos da flag)
  unsigned short base_low;    // Primeros 16 bits da base
  unsigned char  base_middle; // 8 bits da base
  unsigned char  access;      // 8 bits de acesso
  unsigned char  flags;       // 8 bits da flag (granularity)
  unsigned char  base_high;   // Ultimos 8 bits da base
}__attribute__((packed)) gdt_descriptor;

void gdt_init();
void descriptor_define_bits(gdt_descriptor* gdt, unsigned int base, unsigned int limit, unsigned char access, unsigned char flags);

#endif
