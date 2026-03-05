ASM = nasm

SRC = bootloader/boot.asm
BIN = build/boot.bin

all: $(BIN)

$(BIN): $(SRC)
	$(ASM) -f bin $(SRC) -o $(BIN)

run: $(BIN)
	qemu-system-i386 -drive format=raw,file=$(BIN)

clean:
	rm -f $(BIN)

.PHONY: all run clean
