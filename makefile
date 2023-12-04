INCLUDE_PATH = include
LINKER_SCRIPT = link.ld
KASM = bin/kasm.o
KC = bin/kc.o
C_SOURCES = $(wildcard src/*.c)
C_OBJECTS = $(patsubst src/%.c,bin/%.o,$(C_SOURCES))
OBJ = $(wildcard bin/*.o)
GCC_ARGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector -I$(INCLUDE_PATH)
GCC_ARGS_KERNEL = -c kernel.c -o $(KC) $(GCC_ARGS)
LD_ARGS = -m elf_i386 -T $(LINKER_SCRIPT) -o bin/kernel.elf $(OBJ)

default: run

bin/kasm.o: kernel.asm
	nasm -f elf32 kernel.asm -o $(KASM)

bin/kc.o: kernel.c
	gcc $(GCC_ARGS_KERNEL)

# Rule to compile each C source file to its corresponding object file
$(C_OBJECTS): bin/%.o : src/%.c
	gcc -c $< -o $@ $(GCC_ARGS)

bin/boot.elf: $(C_OBJECTS) $(KC) $(KASM) link.ld
	ld $(LD_ARGS)

run: bin/boot.elf
	qemu-system-i386 -kernel bin/kernel.elf 
#-device ahci,id=ahci -drive format=raw,id=disk0,if=none,file=sata0.img -device ide-hd,drive=disk0,bus=ahci.0

iso: bin/boot.elf
	mkdir -p iso/boot/grub
	cp bin/kernel.elf iso/boot/kernel.elf
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o bin/kernel.iso iso

clean:
	rm -rf bin/*.o bin/*.elf