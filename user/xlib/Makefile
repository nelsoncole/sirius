CFLAGS=-Wall -O0 -m32 -ffreestanding -nostdlib -nostdinc -I ./include
.PHONY: xlib clean move

xlib:
	gcc ${CFLAGS} -c libc/errno/*.c
	gcc ${CFLAGS} -c libc/ctype/*.c
	gcc ${CFLAGS} -c libc/time/*.c
	gcc ${CFLAGS} -c libc/stdio/*.c
	gcc ${CFLAGS} -c libc/stdlib/*.c
	gcc ${CFLAGS} -c libc/string/*.c
	gcc ${CFLAGS} -c libc/unistd/*.c
	gcc ${CFLAGS} -c sys/*.c
	gcc ${CFLAGS} -c gui/*.c
	gcc ${CFLAGS} -c fs/*.c	
	gcc ${CFLAGS} -c sys/stdx/*.c
	ar -rcs xlib.a *.o
	gcc -shared -fPIC -o xlib.so *.o
	gcc ${CFLAGS} -c sys/crt0/*.c
	nasm -f elf32 -O0 sys/setup.asm
move:
	mv sys/*setup.o obj
	mv *.o obj

clean:
	rm *.o
