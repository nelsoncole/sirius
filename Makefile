AS=nasm


.PHONY: clearscreen stage0.bin create disk mount push test

#compiler boot
stage0.bin: stage0/stage0.asm
	$(AS) -f bin -o $@ $<
move:
	mv *.bin bin
clean:
	rm bin/*.bin
	rm bin/*.sys

create:
	qemu-img create -f raw part.img 48M
disk:
	qemu-img create -f raw disk.img 512M

mount:
	#sudo ./kmbr bin/stage0.bin /dev/sdb
	mkdosfs part.img
	sudo mount part.img /mnt
	sudo cp bin/*.bin /mnt/
	sudo cp bmp/*.bmp /mnt/
	sudo cp bmp/*.ttf /mnt/
	sudo cp bmp/*.psf /mnt/
	sudo cp bmp/*.tf /mnt/
	sudo cp bmp/*.otf /mnt/
	sudo cp bmp/*.jpg /mnt/
	sudo cp bmp/*.ppm /mnt/
	sudo cp file.txt /mnt/
	sudo cp main.asm /mnt/
	sudo umount /mnt

	./pcopy bin/stage0.bin part.img sirius.vhd
	rm part.img
test:
	#sudo qemu-system-i386 -m 64 -drive file=/dev/sdb,format=raw
	sudo qemu-system-i386 -m 64 -drive file=sirius.vhd,format=raw -hdb disk.img

clearscreen:
	clear



push:
	git add ./
	git commit -m "commit++"
	git push -u origin master
