#ifndef VGA_H
#define VGA_H

// Constante memória de vídeo
#define VGA_MEMORY_ADDRESS 0x000B8000

// Largura framebuffer linear
#define FB_COL_WIDTH 80

// Mapeando I/O cursor VGA
#define VGA_CURSOR_COMMAND_PORT 0x3D4
#define VGA_CURSOR_DATA_PORT 0x3D5

// Mascaras para porta I/O
#define VGA_COMMAND_HIGH_BYTE 0x0E
#define VGA_COMMAND_LOW_BYTE 0x0F

// Cores
#define VGA_GREEN_COLOR 2
#define VGA_DARKGRAY_COLOR 8

// Cursor virtual
typedef struct {
  unsigned char x;
  unsigned char y;
  unsigned int linear_pos;
} cursor_state;

void cursor_init();
void kprint(char* buff);

#endif
