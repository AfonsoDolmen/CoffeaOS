global load_idt

load_idt:
  mov eax, [esp + 4] ; Captura endereço base da IDT
  lidt [eax]
  ret
