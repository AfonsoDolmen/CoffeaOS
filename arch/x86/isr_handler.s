%macro no_error_code_interrupt_handler 1
global interrupt_handler_%1

interrupt_handler_%1:
  push dword 0  ; Código de erro
  push dword %1 ; Número da interrupção
  jmp common_interrupt_handler
%endmacro

%macro error_code_interrupt_handler 1
global interrupt_handler_%1

interrupt_handler_%1:
  push dword %1 ; Número da interrupção
  jmp common_interrupt_handler
%endmacro

extern isr_handler

common_interrupt_handler:
  pusha ; Empurra para a pilha o estado de todos os registradores
  push esp
  call isr_handler ; Função em C
  add esp, 4
  popa
  add esp, 8
  iret  ; Retorna da interrupção

; Chama a macro
no_error_code_interrupt_handler 0
