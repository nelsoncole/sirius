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
GSERVER	:=gserver.o
XSERVER	:=xserver.o
FILES	:=files.o
_SHELL	:=shell.o
DESKTOP	:=desktop.o
EDIT	:=edit.o
TEST	:=test.o
FONT	:=font.o

TRM	:=trm.o
TASKBAR	:=taskbar.o
LOGIN	:=login.o

DISK	:=disk.o
FORMAT	:=format.o
INIT	:=init.o
PCI	:=pci.o
FASM	:=fasm/fasm.o
NANOJPEG:=nanojpeg.o

C4 	:=c4.o

NASM 	:= nasm.o nasmlib.o ver.o raa.o saa.o rbtree.o float.o insnsa.o insnsb.o assemble.o labels.o hashtbl.o crc64.o parser.o outform.o outlib.o nulldbg.o outbin.o outaout.o outcoff.o outelf.o outelf32.o outelf64.o outobj.o outas86.o outrdf2.o outdbg.o outieee.o outmacho32.o outmacho64.o preproc.o quote.o pptok.o macros.o listing.o eval.o exprlib.o stdscan.o strfunc.o tokhash.o regvals.o regflags.o


H1	:=h1.o
CAL	:=cal.o

.PHONY: msgbox.bin gserver.bin xserver.bin shell.bin desktop.bin disk.bin format.bin init.bin pci.bin font.bin fasm.bin nanojpeg.bin c4.bin nasm.bin trm.bin taskbar.bin login.bin edit.bin test.bin h1.bin cal.bin clean move

h1.bin: $(HEAD) $(H1) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

cal.bin: $(HEAD) $(CAL) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

edit.bin: $(HEAD) $(EDIT) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

login.bin: $(HEAD) $(LOGIN) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

trm.bin: $(HEAD) $(TRM) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

taskbar.bin: $(HEAD) $(TASKBAR) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

nasm.bin: $(HEAD) $(NASM) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^
	
nanojpeg.bin: $(HEAD) $(NANOJPEG) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

fasm.bin: $(HEAD) $(FASM) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

c4.bin: $(HEAD) $(C4) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

font.bin: $(HEAD) $(FONT) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

msgbox.bin: $(HEAD) $(MSGBOX) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

gserver.bin: $(HEAD) $(GSERVER) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

xserver.bin: $(HEAD) $(XSERVER) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

shell.bin: $(HEAD) $(_SHELL) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

desktop.bin: $(HEAD) $(DESKTOP) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^


disk.bin: $(HEAD) $(DISK) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

format.bin: $(HEAD) $(FORMAT) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

init.bin: $(HEAD) $(INIT) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

pci.bin: $(HEAD) $(PCI) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

test.bin: $(HEAD) $(TEST) $(XLIB)
	$(LD) $(LDFLAGS) -o $@ $^

#EDIT
%.o:edit/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#C4
%.o:c4/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#LOCAL
%.o:%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
#TASKBAR
%.o:taskbar/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#MSG BOX
%.o:msgbox/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#SERVER
%.o:server/%.c
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

#UTIL
%.o:util/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
#FONT
%.o:font/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#PCI
%.o:pci/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

NASM_CFLAGS32 :=-Wall -O0 -m32 -ffreestanding -nostdlib -nostdinc -I ./nasm -I ./xlib/include
#NASM
%.o:nasm/%.c
	$(CC) $(NASM_CFLAGS32) -c -o $@ $<
#NASM/OUTPUT
%.o:nasm/output/%.c
	$(CC) $(NASM_CFLAGS32) -c -o $@ $<
#NANOJPEG
NANOJPEG_CFLAGS32 :=-Wall -O0 -std=c99 -D_NJ_EXAMPLE_PROGRAM -m32 -ffreestanding -nostdlib -nostdinc -I ./xlib/include 
%.o:nanojpeg/%.c
	$(CC) $(NANOJPEG_CFLAGS32) -c -o $@ $<

#HP
%.o:hp/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

move:
	-mv *.bin ../bin

clean: move
	rm *.o
