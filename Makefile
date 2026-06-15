OBJECTS = build/loader.o build/kmain.o build/io.o build/vga.o build/serial.o build/klog.o build/load_gdt.o build/gdt.o build/load_idt.o build/idt.o build/isr.o build/isr_handler.o build/pic.o build/io_wait.o build/keyboard.o build/ata.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -c -Iinclude
LDFLAGS = -T arch/x86/link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf
ISO = build/coffeaos.iso

all: iso/boot/kernel.elf

iso/boot/kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o iso/boot/kernel.elf

$(ISO): iso/boot/kernel.elf
	genisoimage -R                              \
	            -b boot/grub/stage2_eltorito    \
	            -no-emul-boot                   \
	            -boot-load-size 4               \
	            -A os                           \
	            -input-charset utf8             \
	            -quiet                          \
	            -boot-info-table                \
	            -o $(ISO)                       \
	            iso

run: $(ISO)
	qemu-system-i386 -serial stdio -cdrom $(ISO) -boot d -hda coffeahdd.img

build/%.o: arch/x86/%.s
	$(AS) $(ASFLAGS) $< -o $@

build/%.o: kernel/%.c
	$(CC) $(CFLAGS) $< -o $@

build/%.o: drivers/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf build/*.o iso/boot/kernel.elf $(ISO)
