ASFLAGS  :=-f elf32 -O0
CFLAGS32 :=-Wall -O0 -m32 -ffreestanding -nostdlib -nostdinc -I ./xlib/include
LDFLAGS  :=-m elf_i386 -T xlib/sys/link.ld
AS=nasm
CC=gcc
LD=ld


HEAD	:=xlib/obj/setup.o xlib/obj/crt0.o
XLIB	:=xlib/xlib.a
LIB	:=xlib/xlib.so

MSGBOX	:=msgbox.o
TASK	:=task.o
GSERVER	:=gserver.o
XSERVER	:=xserver.o
FILES	:=files.o
TERMINAL:=terminal.o
_SHELL	:=shell.o
MOUSE	:=mouse.o
EDIT	:=edit.o
TEST	:=test.o

DISK	:=disk.o
FASM	:=__fasm/fasm.o

C4 	:=c4.o

.PHONY: task.sys msgbox.sys gserver.sys xserver.sys files.sys terminal.sys shell.sys mouse.sys edit.sys test.bin disk.sys fasm.bin c4.bin clean move

fasm.bin: $(HEAD) $(FASM) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

c4.bin: $(HEAD) $(C4) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^
	
task.sys: $(HEAD) $(TASK) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

msgbox.sys: $(HEAD) $(MSGBOX) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

gserver.sys: $(HEAD) $(GSERVER) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

xserver.sys: $(HEAD) $(XSERVER) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

files.sys: $(HEAD) $(FILES) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

terminal.sys: $(HEAD) $(TERMINAL) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

shell.sys: $(HEAD) $(_SHELL) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

mouse.sys: $(HEAD) $(MOUSE) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

edit.sys: $(HEAD) $(EDIT) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

disk.sys: $(HEAD) $(DISK) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

test.bin: $(HEAD) $(TEST) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^


#C4
%.o:c4/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#LOCAL
%.o:%.c
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
#FILES
%.o:files/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#TERMINAL
%.o:terminal/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#SHELL
%.o:shell/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
#MOUSE
%.o:mouse/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#EDIT
%.o:edit/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#UTIL
%.o:util/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

move:
	-mv *.sys ../bin
	-mv *.bin ../bin

clean: move
	rm *.o
