; Exporta label
global outb
global inb
global inw
global insw_buffer
global outsw_buffer

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

insw_buffer: ; Joga dados da porta contida em dx para um buffer
  push edi ; Preserva o valor de EDI

  mov dx,  [esp + 8]  ; Porta
  mov cx,  [esp + 12]  ; Numeros de words a serem lido
  mov edi, [esp + 16] ; Endereço buffer

  rep insw

  pop edi
  ret

outsw_buffer:
  push esi ; Preserva esi

  mov dx, [esp + 8]   ; Porta
  mov cx, [esp + 12]  ; Numero de words
  mov esi, [esp + 16] ; Endereço buffer

  .loop:
    lodsw      ; Envia dado para ax e incrementa esi
    out dx, ax ; Envia dado para a porta

    jmp $+2    ; Atraso

    loop .loop

    pop esi
    ret
