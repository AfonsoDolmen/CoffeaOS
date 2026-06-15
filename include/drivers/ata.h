#ifndef ATA_H
#define ATA_H

#include "../types.h"

// Endereço base para registradores do barramento primário e secundário
#define PRIMARY_ATA_REG_BASE   0x1F0
#define SECONDARY_ATA_REG_BASE 0x170

// Endereço base para registradores de controle do barramento
#define PRIMARY_ATA_CTRL_REG_BASE   0x3F6
#define SECONDARY_ATA_CTRL_REG_BASE 0x376

// Comando para seleção do drive e seu modo
#define ATA_SELECT_CHS_MASTER_COMMAND 0xA0
#define ATA_SELECT_CHS_SLAVE_COMMAND  0xB0

#define ATA_DRIVE_SELECT_LBA (1 << 6)

// Quantidade de barramentos
#define BUS_NUM 2

// Quantidade de dispositivo por barramento (master/slave)
#define DRIVES_PER_BUS 2

// Quantidade de dispositivos ATA
#define ATA_DEVICE_NUM 4

// Mascaras para registrador de status alternativo
#define BIT_ERR  (1 << 0) // Indica erro
#define BIT_DRQ  (1 << 3) // Indica que há dados para serem lidos
#define BIT_DF   (1 << 5) // Drive Fault Error
#define BIT_RDY  (1 << 6) // Indica se o drive está livre para enviar/receber dados
#define BIT_BSY  (1 << 7) // Indica se o drive está ocupado

// Offset de registradores para barramento
typedef enum {
  ATA_DATA_REG          = 0, // R/W
  ATA_ERROR_REG         = 1, // R
  ATA_FEATURES_REG      = 1, // W
  ATA_SECTOR_COUNT_REG  = 2, // R/W
  ATA_LBA_LOW_REG       = 3, // R/W
  ATA_LBA_MIDDLE_REG    = 4, // R/W
  ATA_LBA_HIGH_REG      = 5, // R/W
  ATA_DRIVE_REG         = 6, // R/W
  ATA_STATUS_REG        = 7, // R
  ATA_COMMAND_REG       = 7  // W
} ata_reg_offset;

// Offset de registradores de controle
typedef enum {
  ATA_CTRL_STATUS_REG    = 0, // R
  ATA_CTRL_REG           = 0, // W
  ATA_CTRL_UNIT_INFO_REG = 1  // R
} ata_ctrl_reg_offset;

// Dispositivo ATA
typedef struct {
  unsigned int base;          // Endereço base registradores de barramento
  unsigned int ctrl_base;     // Endereço base registrador de controle
  unsigned char drive_select; // Bit de seleção de drive
  signed int total_sectors;   // Quantidade total de setores LBA
  signed char model[41];      // Modelo do disco rígido
  unsigned short is_exist;    // Indica se o driver está ativo
} ata_device;

void ata_init();
void ata_lba28_read_sectors(uint32_t lba, uint8_t sector_count, uint16_t* buffer);

#endif
