AS=nasm


.PHONY: clearscreen stage0.bin create mount test

#compiler boot
stage0.bin: stage0/stage0.asm
	$(AS) -f bin -o $@ $<
move:
	mv *.bin bin
clean: move

create:
	qemu-img create -f raw part.img 48M

mount:
	#sudo ./kmbr bin/stage0.bin /dev/sdb
	mkdosfs part.img
	sudo mount part.img /mnt
	sudo cp bin/stage0.bin /mnt/
	sudo cp bin/stage1.bin /mnt/
	sudo cp bin/logo.bmp /mnt/
	sudo cp bin/kernel.bin /mnt/
	sudo cp bin/task.sys /mnt/
	sudo cp bin/gserver.sys /mnt/
	sudo cp bin/console.sys /mnt/
	sudo cp bin/files.sys /mnt/
	sudo cp bin/msgbox.sys /mnt/
	sudo umount /mnt

	./pcopy bin/stage0.bin part.img sirius.vhd
	rm part.img
test:
	#sudo qemu-system-i386 -m 64 -drive file=/dev/sdb,format=raw
	sudo qemu-system-i386 -m 64 -drive file=sirius.vhd,format=raw

clearscreen:
	clear
