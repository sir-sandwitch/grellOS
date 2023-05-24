default: run

kasm.o: kernel.asm
	nasm -f elf32 kernel.asm -o kasm.o

kc.o: kernel.c
	gcc -m32 -c kernel.c -o kc.o -ffreestanding -O2 -Wall -Wextra -fno-stack-protector

boot.elf: kasm.o kc.o link.ld
	ld -m elf_i386 -T link.ld -o kernel.elf kasm.o kc.o

run: boot.elf
	qemu-system-i386 -kernel kernel.elf

clean:
	rm -rf *.o *.bin *.iso