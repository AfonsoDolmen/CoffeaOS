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

void ata_lba28_read_sectors(uint32_t lba, uint8_t sector_count, uint16_t* buffer)
{
  klog_info("Reading sector...");

  // Seleciona o drive em modo LBA e envia os 4 bits mais alto do LBA
  outb((disk.base + ATA_DRIVE_REG), disk.drive_select | ATA_DRIVE_SELECT_LBA | ((lba >> 24) & 0x0F));

  // Envia a quantidade de setores a serem lidos
  outb((disk.base + ATA_SECTOR_COUNT_REG), sector_count);

  // Envia os bits LBA
  outb((disk.base + ATA_LBA_LOW_REG), (uint8_t)lba);
  outb((disk.base + ATA_LBA_MIDDLE_REG), (uint8_t)(lba >> 8));
  outb((disk.base + ATA_LBA_HIGH_REG), (uint8_t)(lba >> 16));

  // Envia o comando para ler os setores
  outb((disk.base + ATA_COMMAND_REG), 0x20);

  // Polling via registrador de status (futuramente irei implementar via IRQs)
  uint8_t status = inb((disk.base + ATA_STATUS_REG));

  // Espera o drive desocupar
  while ((inb((disk.base + ATA_STATUS_REG)) & BIT_BSY));
  status = inb((disk.base + ATA_STATUS_REG));

  // Aguarda até o drive estiver pronto para leitura
  while (!(status & BIT_DRQ))
  {
    status = inb((disk.base + ATA_STATUS_REG));

    // Verifica se houve erro
    if (status & BIT_ERR)
    {
      ata_device_output_header_error();
      ata_device_msg_color();
      kprintf("Critical failure during reading. Status: %x\n", status);

      klog_error("Critical failure during reading");

      return;
    }

    // Drive Fault Error
    if (status & BIT_DF)
    {
      ata_device_output_header_error();
      ata_device_msg_color();
      kprintf("Drive Fault Error. Status: %x\n", status);

      klog_error("Drive Fault Error");

      return;
    }
  }

  kprintf("Disk Read Status: %x\n", status);

  // Lê os dados
  for (int i = 0; i < 256; i++)
  {
    // Lê 1 word de cada vez
    buffer[i] = inw((disk.base + ATA_DATA_REG));
  }

  klog_ok("Sector read successfully");
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
      kprintf("ATA Device critical error! Status Register: %x\n", status);

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
  kprint("Drive identified\n");
  
  klog_ok("ATA Drive identified");
  return 1;
}

void detect_ata_device()
{
  uint16_t ata_bus[BUS_NUM] = { PRIMARY_ATA_REG_BASE, SECONDARY_ATA_REG_BASE };
  uint16_t ata_drives[DRIVES_PER_BUS] = { ATA_SELECT_CHS_MASTER_COMMAND, ATA_SELECT_CHS_SLAVE_COMMAND };
  uint16_t ata_ctrl_reg[BUS_NUM] = { PRIMARY_ATA_CTRL_REG_BASE, SECONDARY_ATA_CTRL_REG_BASE };

  klog_info("Detecting ATA device...");

  ata_device_output_header();
  ata_device_msg_color();
  kprint("Detecting ATA device...\n");

  // Percorre os barramentos
  for (uint16_t i = 0; i < BUS_NUM; i++)
  {
    disk.base = ata_bus[i];
    disk.ctrl_base = ata_ctrl_reg[i];

    // Percorre os drives
    for (uint16_t j = 0; j < DRIVES_PER_BUS; j++)
    {
      // Seleciona o drive e lê seu status
      select_ata_device(ata_drives[j]);
      delay_400ns(disk.ctrl_base);
      uint8_t status = inb(disk.ctrl_base);

      // Armazena último drive selecionado
      disk.drive_select = ata_drives[j];

      // Verifica se há um drive no barramento (barramento flutuante)
      if (status != 0x00 && status != 0xFF)
      {
        // Verifica o bit RDY que indica que o drive não está "dormindo"
        if (status & BIT_RDY)
        {
          disk.base      = ata_bus[i];
          disk.ctrl_base = ata_ctrl_reg[i];
          klog_ok("ATA device detected");

          ata_device_output_header();
          ata_device_msg_color();
          kprintf("ATA device detected in %x\n", disk.base);

          return;
        }

        ata_device_output_header();
        ata_device_msg_color();
        kprintf("Device not found at %x\n", disk.base);
      }
    }
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
