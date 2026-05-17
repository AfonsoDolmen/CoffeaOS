#include "../include/io.h"
#include "../include/vga.h"

static cursor_state cursor;

static void cursor_set_hardware(unsigned int pos)
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

void get_cursor_xy(unsigned char* x, unsigned char* y)
{
  // Transforma posição linear em xy
  *x = cursor.x;
  *y = cursor.y;
}

void cursor_update(unsigned int new_pos)
{
  // Atualiza a posição do cursor virtual
  cursor.linear_pos = new_pos;
  cursor.x = new_pos % FB_COL_WIDTH;
  cursor.y = new_pos / FB_COL_WIDTH;

  // Atualiza a posição do cursor
  cursor_set_hardware(new_pos);
}

void cursor_new_line()
{
  // Incrementa a linha e atualiza a posição
  cursor_update((cursor.y + 1) * FB_COL_WIDTH);
}

void cursor_init()
{
  // Inicializa o cursor
  cursor.x = 0;
  cursor.y = 0;
  cursor.linear_pos = 0;
  cursor_set_hardware(0);
}

void print_char_cell(unsigned char character, unsigned char fg, unsigned char bg)
{
  // Aponta para o endereço de vídeo VGA
  char* framebuffer = (char*)VGA_MEMORY_ADDRESS;

  // Captura a posição do cursor
  unsigned int offset = cursor.linear_pos;

  // Organiza byte
  unsigned char color = ((bg << 4) & 0xF0) | (fg & 0x0F);

  framebuffer[offset * 2] = character; // Escreve caractere
  framebuffer[offset * 2 + 1] = color; // Definição de cor do caractere

  // Atualiza a posição do cursor
  cursor_update(cursor.linear_pos + 1);
}

void kprint(char* buff)
{
  // Percorre a string
  while (*buff)
  {
    switch (*buff)
    {
      // \n
      case 0x0A:
        cursor_new_line();
        break;
      default:
        print_char_cell(*buff, VGA_GREEN_COLOR, VGA_DARKGRAY_COLOR);
        break;
    }
    
    buff++;
  }
}
