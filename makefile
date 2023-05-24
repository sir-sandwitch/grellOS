default: run

bin/kasm.o: kernel.asm
	nasm -f elf32 kernel.asm -o bin/kasm.o

bin/kc.o: kernel.c
	gcc -m32 -c kernel.c -o bin/kc.o -ffreestanding -O2 -Wall -Wextra -fno-stack-protector

bin/boot.elf: bin/kasm.o bin/kc.o link.ld
	ld -m elf_i386 -T link.ld -o bin/kernel.elf bin/kasm.o bin/kc.o

run: bin/boot.elf
	qemu-system-i386 -kernel bin/kernel.elf

clean:
	rm -rf bin/*.o bin/*.elf