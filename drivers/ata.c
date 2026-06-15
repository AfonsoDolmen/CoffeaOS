#include "../include/types.h"
#include "../include/drivers/ata.h"
#include "../include/drivers/vga.h"
#include "../include/io.h"
#include "../kernel/klog.h"

static ata_device disk;

void ata_device_msg_color()
{
  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}

void ata_device_output_header_error()
{
  vga_set_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
  kprint("[ATA DEVICE ERROR] ");
}

void ata_device_output_header()
{
  vga_set_color(VGA_COLOR_LIGHTMAGENTA, VGA_COLOR_BLACK);
  kprint("[ATA DEVICE] ");
}

void delay_400ns(unsigned char ctrl_base)
{
  // Delay de 400ns
  inb(ctrl_base);
  inb(ctrl_base);
  inb(ctrl_base);
  inb(ctrl_base);
}

void select_ata_device(unsigned char drive)
{
  outb((disk.base + ATA_DRIVE_REG), drive);
}

uint8_t identify_device(unsigned char drive)
{
  ata_device_output_header();
  ata_device_msg_color();
  kprint("Identifying device...\n");

  // Seleciona o drive (master ou slave)
  select_ata_device(drive);

  // Limpa sectorcount e lba
  outb((disk.base + ATA_SECTOR_COUNT_REG), 0x00);
  outb((disk.base + ATA_LBA_LOW_REG), 0x00);
  outb((disk.base + ATA_LBA_MIDDLE_REG), 0x00);
  outb((disk.base + ATA_LBA_HIGH_REG), 0x00);

  // Envia o comando IDENTIFY
  outb((disk.base + ATA_COMMAND_REG), 0xEC);

  // Lê a porta de status
  delay_400ns(disk.ctrl_base);
  uint8_t status = inb(disk.ctrl_base);

  // Se for igual a 0x00, o drive não existe
  if (status == 0x00)
  {
    ata_device_output_header_error();
    ata_device_msg_color();
    kprint("ATA Drive not exist\n");
  
    klog_error("ATA Drive not exist");
    return 0;
  }

  // Lê o LBA
  uint8_t lba_mid = inb(disk.base + ATA_LBA_MIDDLE_REG);
  uint8_t lba_high = inb(disk.base + ATA_LBA_HIGH_REG);

  // Se for diferente de 0, significa que o dispositivo não é ATA
  if (lba_mid != 0x00 || lba_high != 0x00)
  {
    ata_device_output_header_error();
    ata_device_msg_color();
    kprint("Device isn't compatible to ATA specification\n");
  
    klog_error("Device isn't compatible to ATA specification");
    return 0;
  }

  // Verifica se o drive está ocupado
  while(inb(disk.base + ATA_STATUS_REG) & BIT_BSY);
  status = inb(disk.ctrl_base);

  // Verifica se há bytes estão prontos para serem lidos
  while(!(inb(disk.ctrl_base) & BIT_DRQ))
  {
    status = inb(disk.ctrl_base);

    // Verifica se houve erro
    if (status & BIT_ERR)
    {
      ata_device_output_header_error();
      ata_device_msg_color();
      kprint("ATA Device critical error\n");

      klog_error("ATA Device critical error");
      return 0;
    }
  }

  ata_device_output_header();
  ata_device_msg_color();
  kprintf("Status Register: %x\n", status);

  // Buffer para leitura
  uint16_t buffer[256];
  
  // Lê o retorno do comando IDENTIFY
  for (int i = 0; i < 256; i++)
  {
    buffer[i] = inw(disk.base + ATA_DATA_REG);
  }

  // Extrai a quantidade total de setores
  disk.total_sectors = ((uint32_t)buffer[61] << 16) | buffer[60];

  // Disco rigído existe
  disk.is_exist = 1;

  ata_device_output_header();
  ata_device_msg_color();
  kprintf("Total Sectors: %d\n", disk.total_sectors);

  ata_device_output_header();
  ata_device_msg_color();
  kprint("Driver identified\n");
  
  klog_ok("ATA Driver identified");
  return 1;
}

void detect_ata_device()
{
  klog_info("Detecting ATA device...");

  ata_device_output_header();
  ata_device_msg_color();
  kprint("Detecting ATA device...\n");

  // Lê o registrador de status alternativo
  delay_400ns(PRIMARY_ATA_CTRL_REG_BASE);
  uint8_t ata_primary_bus   = inb(PRIMARY_ATA_CTRL_REG_BASE);
  delay_400ns(SECONDARY_ATA_CTRL_REG_BASE);
  uint8_t ata_secondary_bus = inb(SECONDARY_ATA_CTRL_REG_BASE);

  // Verifica se há um drive no barramento (barramento flutuante)
  if (ata_primary_bus != 0x00)
  {
    disk.base      = PRIMARY_ATA_REG_BASE;
    disk.ctrl_base = PRIMARY_ATA_CTRL_REG_BASE;
    klog_ok("ATA device detected in primary bus");

    ata_device_output_header();
    ata_device_msg_color();
    kprintf("ATA device detected in %x\n", disk.base);

    return;
  }

  if (ata_secondary_bus != 0x00)
  {
    disk.base      = SECONDARY_ATA_REG_BASE;
    disk.ctrl_base = SECONDARY_ATA_CTRL_REG_BASE;
    klog_ok("ATA device detected in secondary bus");

    ata_device_output_header();
    ata_device_msg_color();
    kprintf("ATA device detected in %x\n", disk.base);
    
    return;
  }

  // Nenhum dispositivo ATA encontrado
  klog_error("No ATA device detected");

  ata_device_output_header_error();
  ata_device_msg_color();
  kprint("No ATA device detected\n");
  
  return;
}

void ata_init()
{
  detect_ata_device();

  ata_device_output_header();
  ata_device_msg_color();
  kprint("Identifying Master Drive...\n");
  uint8_t drive = identify_device(ATA_SELECT_CHS_MASTER_COMMAND);

  if (!drive)
  {
    ata_device_output_header();
    ata_device_msg_color();
    kprint("Identifying Slave Drive...\n");
    drive = identify_device(ATA_SELECT_CHS_SLAVE_COMMAND);
  }
}
