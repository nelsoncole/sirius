AS=nasm


.PHONY: clearscreen stage0.bin mount test

#compiler boot
stage0.bin: stage0/stage0.asm
	$(AS) -f bin -o $@ $<
move:
	mv *.bin bin
clean: move

mount:
	sudo ./kmbr bin/stage0.bin /dev/sdb
test:
	sudo qemu-system-i386 -m 64 -vga vmware -drive file=/dev/sdb,format=raw

clearscreen:
	clear
