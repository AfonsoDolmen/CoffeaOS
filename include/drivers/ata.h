#ifndef ATA_H
#define ATA_H

// Endereço base para registradores do barramento primário e secundário
#define PRIMARY_ATA_REG_BASE   0x1F0
#define SECONDARY_ATA_REG_BASE 0x170

// Endereço base para registradores de controle do barramento
#define PRIMARY_ATA_CTRL_REG_BASE   0x3F6
#define SECONDARY_ATA_CTRL_REG_BASE 0x376

#define ATA_SELECT_MASTER_COMMAND 0xA0
#define ATA_SELECT_SLAVE_COMMAND  0xB0

// Quantidade de barramentos
#define BUS_NUM 2

// Quantidade de dispositivos ATA
#define ATA_DEVICE_NUM 4

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
  unsigned char drive_select; // Bit de seleção de driver
  signed int total_sectors;   // Quantidade total de setores LBA
  signed char model[41];      // Modelo do disco rígido
  bool is_exist;              // Indica se o driver está ativo
} ata_device;

#endif
