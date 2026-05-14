ENTRY (loader)	/* Entry point principal do kernel */

SECTIONS {
  . = 0x00100000;	  /* O kernel deve ser carregado em um endereço maior ou igual a 1mb */

  .text ALIGN (0x1000) :  /* Alinhamento em 4kb */
  {
    *(.text)		  /* Linka todas as seções text de todos os arquvivos obj (.o) */
  }

  .rodata ALIGN (0x1000) :
  {
    *(.rodata*)		  /* Todos os readonly data de todos os arquivos */
  }

  .data ALIGN (0x1000) :  /* Alinhamento em 4kb */
  {
    *(.data)		  /* Todas as seções data de todos os arquivos */
  }

  .bss ALIGN (0x1000) :
  {
    *(COMMON)		  /* Todas as seções COMMON de todos os arquivos */
    *(.bss)		  /* Todas as seções bss de todos os arquivos */
  }
}
