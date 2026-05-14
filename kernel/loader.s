global loader 		    ; Entry point principal

; Cabeçalho identificador para GRUB
MAGIC_NUMBER equ 0x1BADB002 ; Constante do número mágico (GRUB)
FLAGS equ 0x0		    ; Flags para multiboot
CHECKSUM equ -MAGIC_NUMBER  ; Teste de integridade multiboot (MAGIC_NUMBER + FLAGS + CHECKSUM = 0)

section .text:
align 4		 	    ; Garante alinhamento de 4bytes
  dd MAGIC_NUMBER 	    ; Escreve dados do cabeçalho no arquivo final
  dd FLAGS
  dd CHECKSUM

loader:			    ; Ponto de entrada (parte executável)
  mov eax, 0xCAFEBABE

.loop:
  jmp .loop 		    ; Loop
