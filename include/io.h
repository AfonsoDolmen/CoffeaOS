#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

#include "./types.h"

extern void outb(uint16_t port, uint8_t data);
extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);
extern uint16_t insw_buffer(uint16_t port, uint16_t word_count, uint16_t* buffer);

void io_wait();

#endif
