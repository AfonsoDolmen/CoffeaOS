#ifndef VGA_H
#define VGA_H

// Constante memória de vídeo
#define VGA_MEMORY_ADDRESS 0x000B8000

// Largura e altura do framebuffer linear
#define FB_COL_WIDTH  80
#define FB_COL_HEIGHT 25

// Mapeando I/O cursor VGA
#define VGA_CURSOR_COMMAND_PORT 0x3D4
#define VGA_CURSOR_DATA_PORT 0x3D5

// Mascaras para porta I/O
#define VGA_COMMAND_HIGH_BYTE 0x0E
#define VGA_COMMAND_LOW_BYTE 0x0F

// Cores
#define VGA_COLOR_BLACK        0
#define VGA_COLOR_BLUE         1
#define VGA_COLOR_GREEN        2
#define VGA_COLOR_CYAN         3
#define VGA_COLOR_RED          4
#define VGA_COLOR_MAGENTA      5
#define VGA_COLOR_BROWN        6
#define VGA_COLOR_GRAY         7
#define VGA_COLOR_DARKGRAY     8
#define VGA_COLOR_LIGHTBLUE    9
#define VGA_COLOR_LIGHTGREEN   10
#define VGA_COLOR_LIGHTCYAN    11
#define VGA_COLOR_LIGHTRED     12
#define VGA_COLOR_LIGHTMAGENTA 13
#define VGA_COLOR_YELLOW       14
#define VGA_COLOR_WHITE        15

// Cursor virtual
typedef struct {
  unsigned char x;
  unsigned char y;
  unsigned int linear_pos;
} cursor_state;

void vga_init();
void kprint(char* buff);
void vga_set_color(unsigned char fg, unsigned char bg);

#endif
