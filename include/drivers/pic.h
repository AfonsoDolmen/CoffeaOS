#ifndef PIC_H
#define PIC_H

// Mapeia portas PIC 8259
#define PIC_MASTER_COMMAND_PORT 0x20
#define PIC_MASTER_DATA_PORT    0x21
#define PIC_SLAVE_COMMAND_PORT  0xA0
#define PIC_SLAVE_DATA_PORT     0xA1

// Comando para indicar o fim da interrupção
#define PIC_EOI 0x20

// Constantes de inicialização PIC
#define ICW1_ICW4      0x01 // Indica que a porta ICW4 estará presente
#define ICW1_SINGLE    0x02 // Modo cascata
#define ICW1_INTERVAL4 0x04 // 4 bytes de intervalo entre os vetores
#define ICW1_LEVEL     0x08 // Detecta a IRQ através do level alto
#define ICW1_INIT      0x10 // Inicialização

#define ICW4_8086 0x01 // 8086 (x86) mode
#define ICW4_AUTO 0x02 // EOI normal
#define ICW4_BUF_SLAVE  0x08 // Buffer mode slave
#define ICW4_BUF_MASTER 0x0C // Buffer mode master
#define ICW4_SFNM 0x10

#define CASCADE_IRQ 2 // IRQ para acesso ao segundo PIC

void pic_eoi(unsigned char irq);
void pic_init();

#endif
