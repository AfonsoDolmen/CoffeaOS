#include "../include/drivers/keyboard.h"
#include "../include/drivers/vga.h"
#include "../kernel/klog.h"
#include "../include/io.h"

static char_dict scan_char[SCAN_CODE_LENGTH];

static void ps2_wait_write()
{
  // Espera buffer para escrita
  while(inb(PS2_STATUS_PORT) & 0x02);
}

static void ps2_wait_read()
{
  // Espera buffer para leitura
  while(!inb(PS2_STATUS_PORT) & 1);
}

void keyboard_enable_scan()
{
  // Envia comando para habilitar scan
  ps2_wait_write();
  outb(PS2_DATA_PORT, ENABLE_SCAN_CODE);

  ps2_wait_read();
  unsigned char ack = inb(PS2_DATA_PORT);

  // Verifica se o comando foi enviado com sucesso
  if (ack != ACK_BYTE)
  {
  	klog_error("Enable Keyboard Scan Error!\n");
  	return;
  }

  klog_ok("Keyboard Scan Enabled\n");
}

void keyboard_set_scan_code()
{
  // Envia comando para setar o conjunto de scan codes
  ps2_wait_write();
  outb(PS2_DATA_PORT, SET_SCAN_CODE);

  ps2_wait_read();
  inb(PS2_DATA_PORT);

  // Seleciona Scan Code 3
  ps2_wait_write();
  outb(PS2_DATA_PORT, SCAN_CODE_SET);

  ps2_wait_read();
  unsigned char ack = inb(PS2_DATA_PORT);

  // Verifica se o comando foi enviado com sucesso
  if (ack != ACK_BYTE)
  {
  	klog_error("Set Scan Code Error!\n");
  	return;
  }

  klog_ok("Scan Code Set\n");
}

void init_mapping()
{
  // Alimenta dicionário
  for (int i = 0; i < SCAN_CODE_LENGTH; i++)
  {	    
    scan_char[i].scan_code  = scan_code[i];
	scan_char[i].ascii_char = ascii_char[i];
	//kprintf("Scan Code: %x, ", scan_char[i].scan_code);
	//kprintf("ASCII Char: %c\n", scan_char[i].ascii_char);
  }
}

unsigned char get_scan_char(unsigned char scan_code)
{
  for (int i = 0; i < SCAN_CODE_LENGTH; i++)
  {
    // Compara scan code
  	if (scan_char[i].scan_code == scan_code)
  	{
  	  // Retorna caractere ASCII relacionado
  	  return scan_char[i].ascii_char;
  	}
  }
}

void read_char()
{
  // Lê o scan code enviado pelo teclado
  ps2_wait_read();
  unsigned char scan_code = inb(PS2_DATA_PORT);

  unsigned char ascii_char = get_scan_char(scan_code);

  // Ignora release key
  if (scan_code & 0x80)
    return;

  // Ignora o resend byte (não é uma boa prática, mas funciona)
  if (scan_code == RESEND_BYTE)
  	return;

  // Caractere \n
  if (scan_code == 0x1C)
  	kprint("\n");

  // Filtro para caractere ASCII
  if (ascii_char == 0 || ascii_char > 127)
    return;

  // Escreve na tela
  kprintf("%c", ascii_char);
}

void keyboard_init()
{
  keyboard_enable_scan();
  // keyboard_set_scan_code();
  init_mapping();
}
