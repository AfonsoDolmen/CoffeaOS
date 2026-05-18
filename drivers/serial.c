#include "../include/io.h"
#include "../include/drivers/serial.h"

/*
Driver para comunicação serial (dispositivo UART 16550)
Configura as portas da interface COM
*/

// Configura a taxa de bits da porta (115200Hz / divisor)
void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
  // Ativa o bit DLAB para configurar a taxa de bits (baud rate)
  outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);

  // Envia o divisor (16 bits)
  outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
  outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

// Configura a linha da porta
// 8 bits de configuração d | b | prty | s | dl
/*
d     - Habilita/desabilita dlab para acesso a baud rate -> 1 bit
b     - Habilita break control (b = 1, b = 0) -> 1 bit
pprty - Configura paridade -> 3 bits
s     - Stop bit (s = 0 -> 1 stop bit) -> 1 bit
dl    - Descreve o tamanho dos dados (11 = 8 bits) -> 2 bits
0x03 = 00000011
*/
void serial_configure_line(unsigned short com)
{
  // Envia os bits de configuração para porta
  outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

// Configura o buffer (FIFO) de dados
// Bit:       | 7 6 | 5  | 4 | 3   | 2   | 1   | 0 |
// Descrição: | lvl | bs | r | dma | clt | clr | e |
// 0xC7 = 11000111
void serial_configure_buffer(unsigned short com)
{
  outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

// Configura o fluxo Ready To Transmit (RTS) e Data Terminal Ready (DTR)
// RTS e DTS = 1 -> pronto para enviar dados
// Bit:       | 7 | 6 | 5  | 4  | 3   | 2   | 1   | 0   |
// Descrição: | r | r | af | lb | ao2 | ao1 | rts | dtr |
// 0x03 = 00000011
void serial_configure_modem(unsigned short com)
{
  outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

// Verifica se o buffer FIFO de transmissão está vazia
// Bit 5 = 1 significa que está vazia
int serial_is_transmit_fifo_empty(unsigned short com)
{
  // Lê o status do buffer e aplica a mascara
  return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void hardware_send_serial_data(unsigned short com, unsigned char data)
{
  outb(com, data);
}

void serial_write(char* buff, unsigned short com)
{
  // Espera até a fila de transmissão ficar vazia
  while(!serial_is_transmit_fifo_empty(com));

  // Envia os dados
  while(*buff)
  {
    hardware_send_serial_data(com, *buff);
    buff++;
  }
}

void serial_init(unsigned short com)
{
  serial_configure_baud_rate(com, 12); // 115200 / 12 = 9600;
  serial_configure_line(com);
  serial_configure_buffer(com);
  serial_configure_modem(com);
}
