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

%macro irq_handler 1
global irq_handler_%1

irq_handler_%1:
  push dword 0
  push dword %1
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

; Exceções da CPU 
no_error_code_interrupt_handler 0 ; Divide by Zero
no_error_code_interrupt_handler 6 ; Invalid Opcode
no_error_code_interrupt_handler 7 ; Device not Available
error_code_interrupt_handler 8    ; Double Fault
error_code_interrupt_handler 10   ; Invalid TSS
error_code_interrupt_handler 11   ; Segment not Present
error_code_interrupt_handler 12   ; Stack-Segment Fault
error_code_interrupt_handler 13   ; General Protection Fault
error_code_interrupt_handler 14   ; Page Fault

; IRQ teclado
irq_handler 33
