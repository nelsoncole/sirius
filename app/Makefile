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
FILES	:=files.o
TERMINAL:=terminal.o
_SHELL	:=shell.o

.PHONY: task.sys msgbox.sys gserver.sys files.sys terminal.sys shell.sys clean move

task.sys: $(HEAD) $(TASK) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

msgbox.sys: $(HEAD) $(MSGBOX) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

gserver.sys: $(HEAD) $(GSERVER) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

files.sys: $(HEAD) $(FILES) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

terminal.sys: $(HEAD) $(TERMINAL) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

shell.sys: $(HEAD) $(_SHELL) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^


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

move:
	-mv *.sys ../bin

clean: move
	rm *.o
