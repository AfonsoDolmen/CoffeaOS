#ifndef KEYBOARD_H
#define KEYBOARD_H

// Portas dispositivo PS/2
#define PS2_STATUS_PORT 0x64
#define PS2_DATA_PORT   0x60

// Byte de confirmação de comando (ACK)
#define ACK_BYTE 0xFA

// Comandos PS/2
#define ENABLE_SCAN_CODE 0xF4
#define SET_SCAN_CODE    0xF0

#define SCAN_CODE_SET 0x03

// Tamanho da estrutura de dados conversão
#define SCAN_CODE_LENGTH 27
#define CHAR_NUM SCAN_CODE_LENGTH

// Mapeia caracteres
static unsigned char ascii_char[CHAR_NUM] = {
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'ç',
	'z', 'x', 'c', 'v', 'b', 'n', 'm'
};

// Mapeia scan codes
static unsigned char scan_code[SCAN_CODE_LENGTH] = {
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
	0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x2E,
	0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32
};

// Estrutura de dicionario (chave, valor)
typedef struct {
	unsigned char scan_code;
	unsigned char ascii_char;
} char_dict;


void read_char();
void keyboard_init();

#endif
