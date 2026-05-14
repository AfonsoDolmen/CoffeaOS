OBJECTS = build/loader.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T kernel/link.ld -melf_i386
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
	qemu-system-i386 -cdrom $(ISO) -boot d

build/%.o: kernel/%.s
	$(AS) $(ASFLAGS) $< -o $@

build/%.o: kernel/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf build/*.o iso/boot/kernel.elf $(ISO)
