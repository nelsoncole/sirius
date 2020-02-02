ASFLAGS  :=-f elf32 -O0
CFLAGS32 :=-Wall -O0 -m32 -ffreestanding -nostdlib -nostdinc -I ./lib/lib1/include -I ./lib/libc/include
LDFLAGS  :=-m elf_i386 -T lib/lib1/sys/link.ld
AS=nasm
CC=gcc
LD=ld


LIBC	:=lib/libc/obj/stdio.o lib/libc/obj/string.o #libc.a
LIB1	:=lib/lib1/lib1.a
HEAD	:=lib/lib1/obj/setup.o
HEAD2	:=lib/lib1/obj/__main2.o


CONSOLE= console.o shell.o
MSGBOX= msgbox.o
TASK= task.o
GSERVER= gserver.o
FILES= files.o


.PHONY: task.sys msgbox.sys gserver.sys files.sys terminal.sys shell.sys clean move

task.sys: $(HEAD) $(LIB1) $(LIBC) $(TASK)
	$(LD) $(LDFLAGS) -o $@ $^

msgbox.sys: $(HEAD) $(LIB1) $(LIBC) $(MSGBOX)
	$(LD) $(LDFLAGS) -o $@ $^

gserver.sys: $(HEAD) $(LIB1) $(LIBC) $(GSERVER)
	$(LD) $(LDFLAGS) -o $@ $^

files.sys: $(HEAD) $(LIB1) $(LIBC) $(FILES)
	$(LD) $(LDFLAGS) -o $@ $^

terminal.sys: $(HEAD) $(LIB1) $(LIBC) terminal.o
	$(LD) $(LDFLAGS) -o $@ $^

shell.sys: $(HEAD) $(HEAD2) $(LIBC) shell.o
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
