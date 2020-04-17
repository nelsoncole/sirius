ASFLAGS=-f elf32
CFLAGS32=-Wall -Os -m32 -ffreestanding \
-fno-stack-protector -nostdlib -nostdinc -I ./include #-fomit-frame-pointer -fno-align-functions
LDFLAGS =-m elf_i386 -T link.ld
AS=nasm
CC=gcc
LD=ld

OBJS= stage1.o io16.o vesa.o gdt.o main.o flush.o idt.o isrs.o exceptions.o pic.o irqs.o string.o mboot.o gui.o font8x16.o bmp.o console.o pci.o ata.o ahci.o disk.o fat.o media.o gpu.o

.PHONY: clearscreen stage1.bin move clean

#compiler boot
stage1.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

#STAGE1
%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#GUI
%.o: gui/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#DISK
%.o: disk/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
#FS
%.o: fs/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

move:
	-mv *.bin ../bin
clean: move
	rm *.o

clearscreen:
	clear
