; Exporta label
global outb
global inb
global inw

outb:
  ; Captura argumentos passado pela função C
  mov al, [esp + 8]
  mov dx, [esp + 4]

  out dx, al
  ret

inb:
  mov dx, [esp + 4]

  ; Limpa registrador
  xor eax, eax

  in al, dx
  ret

inw: ; Lê 16 bits
  mov dx, [esp + 4]

  xor eax, eax

  in ax, dx
  ret
