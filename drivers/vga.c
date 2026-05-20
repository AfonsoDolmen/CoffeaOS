#include "../include/io.h"
#include "../include/drivers/vga.h"
#include "../kernel/klog.h"

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

void clear(unsigned char fg, unsigned char bg)
{
  char* framebuffer = (char*)VGA_MEMORY_ADDRESS;

  unsigned char color = ((bg << 4) & 0xF0) | (fg & 0xF0);

  unsigned int screen_size = FB_COL_WIDTH * FB_COL_HEIGHT;

  // Percorre a tela e limpa
  for (unsigned int i = 0; i < screen_size; i++)
  {
    framebuffer[i * 2] = ' ';
    framebuffer[i * 2 + 1] = color;
  }

  // Reseta cursor
  cursor_update(0);
}

void scroll()
{
  char* framebuffer = (char*)VGA_MEMORY_ADDRESS;

  // Verifica se o cursor está na ultima linha
  if (cursor.y >= FB_COL_HEIGHT)
  {
    klog_ok("Scroll activated");
    
    // Move as linhas para cima
    for (unsigned int y = 0; y < FB_COL_HEIGHT-1; y++)
    {
      for (unsigned int x = 0; x < FB_COL_WIDTH; x++)
      {
        // Calcula a posição linear do indice
        framebuffer[(y * FB_COL_WIDTH) + x] = framebuffer[((y+1) * FB_COL_WIDTH) + x];
      }
    }

    klog_ok("Linha copiada");

    // Offset da ultima linha
    unsigned int offset = (FB_COL_HEIGHT - 1) * FB_COL_WIDTH;

    // Caractere padrão
    unsigned int color = ((VGA_COLOR_WHITE << 4) & 0x0F) | (VGA_COLOR_WHITE & 0x0F); // Debug

    // Limpa a última linha
    for (unsigned int i = offset; i < (FB_COL_WIDTH * FB_COL_HEIGHT); i++)
    {
      framebuffer[i * 2] = ' ';
      framebuffer[i * 2 + 1] = color;
    }

    // Atualiza posição do cursor
    cursor_update(offset);
  }
}

void print_char_cell(unsigned char character, unsigned char fg, unsigned char bg)
{
  // Aponta para o endereço de vídeo VGA
  char* framebuffer = (char*)VGA_MEMORY_ADDRESS;

  scroll();

  // Captura a posição do cursor
  unsigned int offset = cursor.linear_pos;

  // Organiza byte
  unsigned char color = ((bg << 4) & 0xF0) | (fg & 0x0F);

  framebuffer[offset * 2] = character; // Escreve caractere
  framebuffer[offset * 2 + 1] = color; // Definição de cor do caractere

  // Atualiza a posição do cursor
  cursor_update(cursor.linear_pos + 1);
}

void cursor_init()
{
  // Inicializa o cursor
  cursor.x = 0;
  cursor.y = 0;
  cursor.linear_pos = 0;
  cursor_set_hardware(0);
  clear(VGA_COLOR_BLACK, VGA_COLOR_BLACK);
}

void kprint(char* buff, unsigned char fg, unsigned char bg)
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
        print_char_cell(*buff, fg, bg);
        break;
    }
    
    buff++;
  }
}
