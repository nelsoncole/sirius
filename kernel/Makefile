ASFLAGS  :=-f elf32 -O0
CFLAGS32 :=-Wall -O0 -m32 -ffreestanding -nostdlib -nostdinc -I ./include
LDFLAGS  :=-m elf_i386 -Map Kernel.map -m elf_i386 -T link.ld
AS=nasm
CC=gcc
LD=ld


OBJS= setup.o vector.o main.o os.o data.o dscptor.o exceptions.o string.o gui.o font8x16.o bmp.o console.o cpuid.o apic.o lvt.o irq.o paging.o memory.o thread.o exec.o exit.o syscall.o pci.o ata.o ahci.o disk.o media.o ps2.o mouse.o keyboard.o rtc.o e1000.o process.o lib.o fat.o exectve.o pit.o

.PHONY: kernel.bin clean move clearscreen

kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

#KERNEL
%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS32) -c -o $@ $<
#GUI
%.o:gui/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
#DRIVE
%.o:drive/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#DRIVE/PS2
%.o:drive/ps2/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#DRIVE/RTC
%.o:drive/rtc/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#DRIVE/RTC
%.o:drive/pit/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#DRIVE/DISK
%.o:drive/disk/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#DRIVE/NET
%.o:drive/net/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#FS
%.o:fs/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#LIB
%.o:lib/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

move:
	-mv *.bin ../bin

clean: move
	rm *.o

clearscreen:
	clear

