#include "../include/io.h"
#include "../include/drivers/vga.h"
#include "../kernel/klog.h"

// Variáveis globais
static cursor_state cursor;
static unsigned char color = ((VGA_COLOR_BLACK << 4) & 0xF0) | (VGA_COLOR_WHITE & 0x0F);

static void cursor_set_hardware(unsigned int pos)
{
  outb(VGA_CURSOR_COMMAND_PORT, VGA_COMMAND_HIGH_BYTE);
  outb(VGA_CURSOR_DATA_PORT, (pos >> 8) & 0x0FF);

  outb(VGA_CURSOR_COMMAND_PORT, VGA_COMMAND_LOW_BYTE);
  outb(VGA_CURSOR_DATA_PORT, pos & 0xFF);
}

void scroll()
{
  char* framebuffer = (char*)VGA_MEMORY_ADDRESS;

  // Verifica se o cursor está na ultima linha
  if (cursor.y >= FB_COL_HEIGHT)
  {  
    // Faz varredura na tela
    for (unsigned int y = 0; y < FB_COL_HEIGHT-1; y++)
    {
      for (unsigned int x = 0; x < FB_COL_WIDTH; x++)
      {
        /*
          Caputra o indice e acessa os bytes de caractere e cor
          i = (y * width) + x
        */
        unsigned int current_line = (y * FB_COL_WIDTH) + x;
        unsigned int next_line = ((y + 1) * FB_COL_WIDTH) + x;

        framebuffer[current_line * 2] = framebuffer[next_line * 2];         // Caractere
        framebuffer[current_line * 2 + 1] = framebuffer[next_line * 2 + 1]; // Cor
      }
    }

    // Offset da ultima linha
    unsigned int offset = FB_COL_WIDTH * (FB_COL_HEIGHT - 1);

    // Caractere padrão
    unsigned int color = ((VGA_COLOR_BLACK << 4) & 0x0F) | (VGA_COLOR_BLACK & 0x0F);

    // Limpa a última linha
    for (unsigned int i = 0; i < FB_COL_WIDTH; i++)
    {
      framebuffer[(i + offset) * 2] = ' ';
      framebuffer[(i + offset) * 2 + 1] = color;
    }

    // Atualiza posição do cursor para o inicio da ultima linha
    cursor.x = 0;
    cursor.y = FB_COL_HEIGHT - 1;
    cursor.linear_pos = (cursor.y * FB_COL_WIDTH) + cursor.x;

    cursor_set_hardware(cursor.linear_pos);
  }
}

void cursor_update(unsigned int new_pos)
{
  // Atualiza a posição do cursor virtual
  cursor.linear_pos = new_pos;
  cursor.x = new_pos % FB_COL_WIDTH;
  cursor.y = new_pos / FB_COL_WIDTH;

  scroll();

  // Atualiza a posição do cursor
  cursor_set_hardware(cursor.linear_pos);
}

void cursor_return()
{
  // Move o cursor para inicio da linha
  cursor.x = 0;
  cursor.linear_pos = (cursor.y * FB_COL_WIDTH) + cursor.x;

  // Atualiza cursor
  cursor_update(cursor.linear_pos);
}

void cursor_tab()
{
  // Calcula o próximo tab stop (multiplo de 4)
  cursor.x += 4 - (cursor.x % 4);

  cursor.linear_pos = (cursor.y * FB_COL_WIDTH) + cursor.x;

  // Atualiza cursor
  cursor_update(cursor.linear_pos);
}

void cursor_new_line()
{
  // Incrementa a linha e atualiza a posição
  cursor_update((cursor.y + 1) * FB_COL_WIDTH);
}

void clear()
{
  char* framebuffer = (char*)VGA_MEMORY_ADDRESS;

  unsigned char clear_color = color;

  unsigned int screen_size = FB_COL_WIDTH * FB_COL_HEIGHT;

  // Percorre a tela e limpa
  for (unsigned int i = 0; i < screen_size; i++)
  {
    framebuffer[i * 2] = ' ';
    framebuffer[i * 2 + 1] = clear_color;
  }

  // Reseta cursor
  cursor_update(0);
}

void print_char_cell(unsigned char character)
{
  // Aponta para o endereço de vídeo VGA
  char* framebuffer = (char*)VGA_MEMORY_ADDRESS;

  // Captura a posição do cursor
  unsigned int offset = cursor.linear_pos;

  framebuffer[offset * 2] = character; // Escreve caractere
  framebuffer[offset * 2 + 1] = color; // Definição de cor do caractere

  // Atualiza a posição do cursor
  cursor_update(cursor.linear_pos + 1);
}

void verify_special_char(unsigned char c)
{
  switch (c)
  {
    // \t
    case 0x09:
      cursor_tab();
      break;
      // \n
    case 0x0A:
      cursor_new_line();
      break;
    // \r
    case 0x0D:
      cursor_return();
      break;
      
    default:
      print_char_cell(c);
      break;
  }
}

void delete_char() // Gambiarra temporária (prometo)
{
  // Move o cursor para trás
  if (cursor.x != 0)
  	cursor.x -= 1;

  // Calcula posição linear
  unsigned int new_pos  = (cursor.y * FB_COL_WIDTH) + cursor.x;
  cursor_update(new_pos);

  // Escreve um caractere vazio
  print_char_cell(' ');

  cursor.x -= 1;
  new_pos = (cursor.y * FB_COL_WIDTH) + cursor.x;

  cursor_update(new_pos);
}

void kprint(char* buff)
{
  // Percorre a string
  while (*buff)
  {
    verify_special_char(*buff);
    buff++;
  }
}

void format_print_string(char* str)
{
  int i = 0;

  while (str[i] != '\0')
  {
    print_char_cell((unsigned char)str[i]);
    i++;
  }
}

void format_print_hex(unsigned int value)
{
  if (value == 0)
  {
    print_char_cell('0');
    return;
  }

  // Mapeia base hexadecimal
  char hex_digits[] = "0123456789ABCDEF";

  // Buffer temporário para conversão
  char buffer_value[9];
  int i = 0;

  while (value > 0)
  {
    int digit = value % 16;
    buffer_value[i++] = hex_digits[digit];
    value = value / 16;
  }

  while (i > 0)
  {
    print_char_cell(buffer_value[--i]);
  }
}

void format_print_int(int value)
{
  if (value == 0)
  {
    print_char_cell('0');
    return;
  }

  if (value < 0)
  {
    value = -value;
    print_char_cell('-');
  }

  // Cria buffer temporário para conversão
  char buffer_value[12];
  int i = 0;

  while (value > 0)
  {
    // Isola o ultimo digito do número (ex: "123" % 10 = '3')
    int digit = value % 10;

    // Converte para ASCII e armazena no buffer
    buffer_value[i++] = digit + '0';

    value = value / 10;
  }

  // Escreve o número de trás para frente
  while (i > 0)
  {
    print_char_cell(buffer_value[--i]);
  }
}

void kprintf(char* buff, void* arg)
{
  int i = 0;
  
  // Percorre a string
  while (buff[i] != '\0')
  {
    if (buff[i] != '%')
    {
      // Escreve caractere
      verify_special_char(buff[i]);
      i++;
      continue;
    }

    // Vai para o próximo caractere
    i++;

    switch (buff[i])
    {
      // Números
      case 'd':
        format_print_int((int)arg);
        break;

      case 'c':
        print_char_cell((unsigned char)arg);
        break;

      case 's':
        format_print_string((char*)arg);
        break;

      case 'x':
        // Prefixos hexadecimal
        print_char_cell('0');
        print_char_cell('x');
        format_print_hex((unsigned int)arg);
        break;

      default:
        print_char_cell('%');
        break;
    }

    i++;
  }
}

void vga_set_color(unsigned char fg, unsigned char bg)
{
  // Atualiza a cor
  color = ((bg << 4) & 0xF0) | (fg & 0x0F);
}

void vga_init()
{
  // Inicializa o cursor
  cursor.x = 0;
  cursor.y = 0;
  cursor.linear_pos = 0;
  cursor_set_hardware(0);
  clear();
}
