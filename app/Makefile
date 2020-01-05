ASFLAGS  :=-f elf32 -O0
CFLAGS32 :=-Wall -O0 -m32 -ffreestanding -fno-stack-protector -nostdlib -nostdinc -fno-align-functions -I ./lib/include
LDFLAGS  := -m elf_i386 -T lib/sys/link.ld
AS=nasm
CC=gcc
LD=ld


CONSOLE= setup.o __main.o console.o shell.o string.o gui.o font8x16.o bmp.o
MSGBOX= setup.o __main.o msgbox.o string.o gui.o font8x16.o bmp.o
TASK= setup.o __main.o task.o string.o gui.o font8x16.o bmp.o
GSERVER= setup.o __main.o gserver.o string.o gui.o font8x16.o bmp.o


.PHONY: console.sys task.sys msgbox.sys gserver.sys clean move

console.sys: $(CONSOLE)
	$(LD) $(LDFLAGS) -o $@ $^

task.sys: $(TASK)
	$(LD) $(LDFLAGS) -o $@ $^

msgbox.sys: $(MSGBOX)
	$(LD) $(LDFLAGS) -o $@ $^

gserver.sys: $(GSERVER)
	$(LD) $(LDFLAGS) -o $@ $^

#LIB/SYS
%.o:lib/sys/%.asm
	$(AS) $(ASFLAGS) $< -o $@

%.o:lib/sys/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
#GUI
%.o:lib/gui/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#LIB
%.o:lib/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#CONSOLE
%.o:console/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#TASK
%.o:task/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#MSG BOX
%.o:msgbox/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#SERVER
%.o:server/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

move:
	-mv *.sys ../bin

clean: move
	rm *.o
