#ifndef SERIAL_H
#define SERIAL_H

// Porta base COM1
#define SERIAL_COM1_BASE 0x3F8

// Registradores de cada porta COM pode ser calculada utilizando uma base
#define SERIAL_DATA_PORT(base) (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2) // Diferença de 1 byte
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

// Habilita o bit DLAB para configuração da taxa de bits
#define SERIAL_LINE_ENABLE_DLAB 0x80

void serial_write(char* buff, uint16_t com);
void serial_init(uint16_t com);

#endif
