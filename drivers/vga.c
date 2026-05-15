#include "../include/io.h"

// Constante memória de vídeo
#define VGA_MEMORY_ADDRESS 0x000B8000

// Mapeando I/O cursor VGA
#define VGA_CURSOR_COMMAND_PORT 0x3D4
#define VGA_CURSOR_DATA_PORT 0x3D5

#define VGA_COMMAND_HIGH_BYTE 0x0E
#define VGA_COMMAND_LOW_BYTE 0x0F

// Cores
#define VGA_GREEN_COLOR 2
#define VGA_DARKGRAY_COLOR 8

void update_cursor_pos(unsigned int pos)
{
  outb(VGA_CURSOR_COMMAND_PORT, VGA_COMMAND_HIGH_BYTE);
  outb(VGA_CURSOR_DATA_PORT, (pos >> 8) & 0x0FF);

  outb(VGA_CURSOR_COMMAND_PORT, VGA_COMMAND_LOW_BYTE);
  outb(VGA_CURSOR_DATA_PORT, pos & 0xFF);
}

unsigned int get_cursor_pos()
{
  // 16 bits (2 bytes)
  unsigned int pos = 0;

  // Captura o byte mais significativo da posição
  outb(VGA_CURSOR_COMMAND_PORT, VGA_COMMAND_HIGH_BYTE);
  pos |= (unsigned int)inb(VGA_CURSOR_DATA_PORT) << 8;

  // Captura o byte menos significativo da posição
  outb(VGA_CURSOR_COMMAND_PORT, VGA_COMMAND_LOW_BYTE);
  pos |= (unsigned int)inb(VGA_CURSOR_DATA_PORT);

  return pos;
  
}

void print_char_cell(unsigned char character, unsigned char fg, unsigned char bg)
{
  // Aponta para o endereço de vídeo VGA
  char* framebuffer = (char*)VGA_MEMORY_ADDRESS;

  // Captura a posição do cursor
  unsigned int offset = get_cursor_pos();

  // Organiza byte
  unsigned char color = ((bg << 4) & 0xF0) | (fg & 0x0F);

  framebuffer[offset * 2] = character; // Escreve caractere
  framebuffer[offset * 2 + 1] = color; // Definição de cor do caractere

  // Próxima posição do cursor
  offset++;

  // Atualiza a posição do cursor
  update_cursor_pos(offset);
}

void kprint(char* buff)
{
  // Percorre a string
  while (*buff)
  {
    print_char_cell(*buff, VGA_DARKGRAY_COLOR, VGA_GREEN_COLOR);
    
    buff++;
  }
}
