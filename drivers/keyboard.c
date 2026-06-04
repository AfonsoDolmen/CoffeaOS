#include "../include/drivers/keyboard.h"
#include "../include/drivers/vga.h"
#include "../kernel/klog.h"
#include "../include/io.h"

static char_dict scan_char[SCAN_CODE_LENGTH];
static keyboard_state state;

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
  	klog_error("Enable Keyboard Scan Error!");
  	return;
  }

  klog_ok("Keyboard Scan Enabled");
}

void init_mapping()
{
  // Alimenta dicionário
  for (int i = 0; i < SCAN_CODE_LENGTH; i++)
  {	    
    scan_char[i].scan_code  = scan_code[i];
	  scan_char[i].ascii_char = ascii_char[i];
  }
}

unsigned char get_scan_char(unsigned char scan_code)
{
  for (int i = 0; i < SCAN_CODE_LENGTH; i++)
  {
    // Compara scan code
  	if (scan_char[i].scan_code == scan_code)
  	  return scan_char[i].ascii_char;
  }

  // Caso a tecla não esteja mapeada, retorne 0
  return 0x00;
}

void toggle_led(unsigned char led)
{
  // Envia o comando para alterar estado do led
  ps2_wait_write();
  outb(PS2_DATA_PORT, SET_LED_COMMAND);

  // Lê ACK byte
  ps2_wait_read();
  unsigned char led_response = inb(PS2_DATA_PORT);

  if (led_response != ACK_BYTE)
  {
    klog_error("Set keyboard led command error");
    return;
  }

  // Altera o estado do led
  ps2_wait_write();
  outb(PS2_DATA_PORT, led);

  ps2_wait_read();
  led_response = inb(PS2_DATA_PORT);

  if (led_response != ACK_BYTE)
  {
    klog_error("Toggle led state error");
    return;
  }
}

void toggle_capslock()
{
  if(!state.caps_lock)
  {
    state.caps_lock = 0x01;
    toggle_led(CAPS_LOCK_LED);
    return;
  }else
  {
    state.caps_lock = 0x00;
    toggle_led(CAPS_LOCK_LED);
    return;
  }
}

void toggle_numlock()
{
  if (!state.num_lock)
  {
    state.num_lock = 0x01;
    toggle_led(NUM_LOCK_LED);
    return;
  }else
  {
    state.num_lock = 0x00;
    toggle_led(NUM_LOCK_LED);
    return;
  }
}

void toggle_scrolllock()
{
  // Verifica estado
  if (!state.scroll_lock)
  {
    state.scroll_lock = 0x01;
    toggle_led(SCROLL_LOCK_LED);
    return;
  }else
  {
    state.scroll_lock = 0x00;
    toggle_led(SCROLL_LOCK_LED);
    return;
  }
}

void toggle_keys(unsigned char* scan_code)
{
  switch(*scan_code)
  {
    // Scroll Lock
    case 0x46: toggle_scrolllock(); break;
    // Num Lock
    case 0x45: toggle_numlock(); break;
    // Caps Lock
    case 0x3A: toggle_capslock(); break;
  }
}

void read_char()
{
  // Lê o scan code enviado pelo teclado
  ps2_wait_read();
  unsigned char scan_code = inb(PS2_DATA_PORT);

  unsigned char ascii_char = get_scan_char(scan_code);

  // Ignora o resend byte (não é uma boa prática, mas funciona)
  if (scan_code == RESEND_BYTE)
    return;

  // Verifica teclas scroll lock, num lock e caps lock
  toggle_keys(&scan_code);

  // Ignora release key
  if (scan_code & 0x80)
    return;

  // Backspace
  if (scan_code == 0x0E)
  {
  	delete_char();
    return;
  }

  // Caractere \n
  if (scan_code == 0x1C)
  {
    kprint("\n");
    return;
  }

  // Filtro para caractere ASCII
  if (ascii_char == 0 || ascii_char > 127)
    return;

  // Escreve na tela
  kprintf("%c", ascii_char);
}

void keyboard_init()
{
  // Zera estado
  state.scroll_lock = 0x00; state.num_lock = 0x00; state.caps_lock = 0x00;

  keyboard_enable_scan();
  init_mapping();
}
