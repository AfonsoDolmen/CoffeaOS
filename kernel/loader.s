global loader 		    ; Entry point principal

; Cabeçalho identificador para GRUB
MAGIC_NUMBER equ 0x1BADB002 ; Constante do número mágico (GRUB)
FLAGS equ 0x0		    ; Flags para multiboot
CHECKSUM equ -MAGIC_NUMBER  ; Teste de integridade multiboot (MAGIC_NUMBER + FLAGS + CHECKSUM = 0)

KERNEL_STACK_SIZE equ 4096  ; Tamanho da pilha (4kb)

; Configurando pilha
section .bss
align 4			    ; Garante alinhamento de 4bytes
kernel_stack:               ; Aponta pra o inicío da memória
  resb KERNEL_STACK_SIZE    ; Reserva a pilha na memória

section .text
align 4
  dd MAGIC_NUMBER 	    ; Escreve dados do cabeçalho no arquivo final
  dd FLAGS
  dd CHECKSUM

loader:			    ; Ponto de entrada (parte executável)
  mov eax, 0xCAFEBABE       ; Movendo valor aleatório para eax

  ; Faz o registrador esp apontar para o final da pilha
  mov esp, kernel_stack + KERNEL_STACK_SIZE

.loop:
  jmp .loop

section .rodata
align 4
  kernel_signature db "CoffeaKernel", 0
