#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

extern void outb(uint16_t port, uint8_t data);
extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);

void io_wait();

#endif
