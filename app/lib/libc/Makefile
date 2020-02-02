CFLAGS=-Wall -O0 -m32 -ffreestanding -nostdlib -nostdinc -I ./include
.PHONY: libc clean move

libc:
	gcc ${CFLAGS} -c *.c
	ar -rcs libc.a *.o
move:
	mv *.o obj

clean:
	rm *.o
