[ORG 0x7C00]     ; Inicia no endereço 0x7C00
BITS 16

start:           ; Limpa registradores
  cli            ; Desativa interrupções
  xor ax, ax
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov sp, 0x7C00 ; Inicia a pilha
  cld            ; Limpa direction flag
  sti            ; ativa interrupções

  mov si, welcome_message

  jmp print_welcome

print_welcome:   ; Lógica para percorrer strings
  lodsb

  or al, al
  jz halt

  mov ah, 0x0E
  int 0x10

  jmp print_welcome

halt:            ; Deixa o sistema em espera
  jmp $

welcome_message db "CoffeaOS", 0

times 510-($-$$) db 0 ; Garante 510 bytes de tamanho
db 0x55, 0xAA         ; Boot signature
