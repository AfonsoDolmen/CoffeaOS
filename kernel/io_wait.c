#include "../include/io.h"

void io_wait()
{
  // Delay de 1 a 4 milissegundos
  outb(0x80, 0);
}
