default: run

bin/kasm.o: kernel.asm
	nasm -f elf32 kernel.asm -o bin/kasm.o

bin/kc.o: kernel.c
	gcc -m32 -c kernel.c -o bin/kc.o -ffreestanding -O2 -Wall -Wextra -fno-stack-protector

bin/boot.elf: bin/kasm.o bin/kc.o link.ld
	ld -m elf_i386 -T link.ld -o bin/kernel.elf bin/kasm.o bin/kc.o

run: bin/boot.elf
	qemu-system-i386 -kernel bin/kernel.elf -device ahci,id=ahci -drive format=raw,id=disk0,if=none,file=sata0.img -device ide-hd,drive=disk0,bus=ahci.0

iso: bin/boot.elf
	mkdir -p iso/boot/grub
	cp bin/kernel.elf iso/boot/kernel.elf
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o bin/kernel.iso iso

clean:
	rm -rf bin/*.o bin/*.elf