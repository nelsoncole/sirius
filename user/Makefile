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
DESKTOP	:=desktop.o
EDIT	:=edit.o
TEST	:=test.o
FONT	:=font.o

DISK	:=disk.o
FORMAT	:=format.o
INIT	:=init.o
PCI	:=pci.o
FASM	:=fasm/fasm.o
NANOJPEG:=nanojpeg.o

C4 	:=c4.o

NASM 	:= nasm.o nasmlib.o float.o insnsa.o assemble.o labels.o parser.o outform.o outbin.o outaout.o outcoff.o outelf32.o outelf64.o outobj.o outas86.o outrdf2.o outdbg.o outieee.o outmacho.o preproc.o listing.o eval.o

.PHONY: task.sys msgbox.sys gserver.sys xserver.sys files.sys terminal.sys shell.sys desktop.sys edit.sys test.bin disk.sys format.sys init.sys pci.sys font.sys fasm.bin nanojpeg.bin c4.bin nasm.bin clean move


nasm.bin: $(HEAD) $(NASM) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^
	
nanojpeg.bin: $(HEAD) $(NANOJPEG) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

fasm.bin: $(HEAD) $(FASM) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

c4.bin: $(HEAD) $(C4) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

font.sys: $(HEAD) $(FONT) $(XLIB)
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

desktop.sys: $(HEAD) $(DESKTOP) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

edit.sys: $(HEAD) $(EDIT) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

disk.sys: $(HEAD) $(DISK) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

format.sys: $(HEAD) $(FORMAT) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

init.sys: $(HEAD) $(INIT) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

pci.sys: $(HEAD) $(PCI) $(XLIB)
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
#DESKTOP
%.o:desktop/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#EDIT
%.o:edit/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#UTIL
%.o:util/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
#FONT
%.o:font/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#PCI
%.o:pci/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
NASM_CFLAGS32 :=-Wall -O2 -pedantic -DHAVE_CONFIG_H -W -ffreestanding -nostdlib -nostdinc -I ./xlib/include
#NASM
%.o:nasm/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
#NASM/OUTPUT
%.o:nasm/output/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
#NANOJPEG
NANOJPEG_CFLAGS32 :=-Wall -O0 -std=c99 -D_NJ_EXAMPLE_PROGRAM -m32 -ffreestanding -nostdlib -nostdinc -I ./xlib/include 
%.o:nanojpeg/%.c
	$(CC) $(NANOJPEG_CFLAGS32) -c -o $@ $<


move:
	-mv *.sys ../bin
	-mv *.bin ../bin

clean: move
	rm *.o
