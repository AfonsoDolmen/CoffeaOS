global load_gdt

load_gdt:
  mov eax, [esp + 4] ; Captura o argumento passado via C
  lgdt [eax]         ; Carrega a estrutura GDT na CPU

  jmp 0x08:flush_registers ; CS = 0x08 (base do descritor do segmento de código)

flush_registers:
  xor eax, eax ; Limpa registrador

  mov ax, 0x10 ; Offset para descritor de segmento dos dados

  ; Move descriptor selector para os registradores de segmento
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  ret
