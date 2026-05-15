; Exporta label
global outb
global inb

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
