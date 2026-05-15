#define VGA_VIDEO_MEMORY 0x000B8000;

#define GREEN_COLOR 2 // 0010
#define GRAY_COLOR 8  // 1000

void kmain()
{
  char* ptr = (char*)VGA_VIDEO_MEMORY;

  // 4 bits mais significativos -> bg
  // 4 bits menos significativos -> fg
  // color = 00101000
  char color = ((GREEN_COLOR << 4) & 0x0F) | (GRAY_COLOR & 0x0F);

  ptr[0] = 'H'; // Caractere -> 1 byte
  ptr[1] = color; // (fg, bg) -> 1 byte

  ptr[2] = 'i';
  ptr[3] = color;

  while(1) { } // Halt
}
