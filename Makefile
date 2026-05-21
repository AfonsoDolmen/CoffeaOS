OBJECTS = build/loader.o build/kmain.o build/io.o build/vga.o build/serial.o build/klog.o build/load_gdt.o build/gdt.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c -Iinclude
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
	qemu-system-i386 -serial stdio -cdrom $(ISO) -boot d

build/%.o: arch/x86/%.s
	$(AS) $(ASFLAGS) $< -o $@

build/%.o: kernel/%.c
	$(CC) $(CFLAGS) $< -o $@

build/%.o: drivers/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf build/*.o iso/boot/kernel.elf $(ISO)
