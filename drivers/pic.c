#include "../include/drivers/pic.h"
#include "../include/io.h"
#include "../include/types.h"
#include "../kernel/klog.h"

void IRQ_set_mask(uint8_t irq)
{
  uint16_t port; // Porta para PIC (master/slave)
  uint8_t value;

  // Verifica se a IRQ pertence ao master ou slave
  if (irq < 8)
  {
    port = PIC_MASTER_DATA_PORT;
  } else
  {
    port = PIC_SLAVE_DATA_PORT;
    irq -= 8;
  }

  // Lê a mascara atual do IMR e ativa o IRQ desejada
  value = inb(port) | (1 << irq);
  outb(port, value);

  // Verifica se a IRQ foi ativada
  if (inb(port) & (1 << irq))
    klog_ok("IRQ activated");
  else
    klog_error("IRQ set mask error");
}

void IRQ_clear_mask(uint8_t irq)
{
  uint16_t port;
  uint8_t value;

  if (irq < 8)
  {
    port = PIC_MASTER_DATA_PORT;
  } else
  {
    port = PIC_SLAVE_DATA_PORT;
    irq -= 8;
  }

  // Lê a mascara atual do IMR e desativa a IRQ desejada 
  value = inb(port) & ~(1 << irq);
  outb(port, value);

  // Verifica se a IRQ foi desativada
  if ((inb(port) & ~(1 << irq)))
    klog_ok("IRQ desactivated");
  else
    klog_error("IRQ clear mask error");
}

void pic_eoi(uint8_t irq)
{
  // Verifica de qual PIC o IRQ veio
  if (irq >= 8)
      outb(PIC_SLAVE_COMMAND_PORT, PIC_EOI);

  // Avisa ao dispositivo que a interrupção foi tratada e finalizada
  outb(PIC_MASTER_COMMAND_PORT, PIC_EOI);
}

void pic_remap(uint32_t offset1, uint32_t offset2)
{
  // Inicialização em cascata
  outb(PIC_MASTER_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);
  io_wait();
  outb(PIC_SLAVE_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);

  // Offset do vetor para PIC1
  outb(PIC_MASTER_DATA_PORT, offset1);
  io_wait();
  
  // Offset do vetor para PIC2
  outb(PIC_SLAVE_DATA_PORT, offset2);
  io_wait();

  // Diz ao PIC principal que há um slave na porta IRQ2
  outb(PIC_MASTER_DATA_PORT, 1 << CASCADE_IRQ);
  io_wait();

  outb(PIC_SLAVE_DATA_PORT, CASCADE_IRQ);
  io_wait();

  // 8086 mode
  outb(PIC_MASTER_DATA_PORT, ICW4_8086);
  io_wait();
  outb(PIC_SLAVE_DATA_PORT, ICW4_8086);
  io_wait();

  // Mascara todas as IRQs
  outb(PIC_MASTER_DATA_PORT, 0xFF);
  outb(PIC_SLAVE_DATA_PORT, 0xFF);
}

void pic_init()
{
  klog_info("Setting up PIC...");
  pic_remap(0x20, 0x28);

  // Ativa IRQ
  IRQ_clear_mask(1); // Ativa IRQ teclado

  klog_ok("PIC1 and PIC2 set up");
}
