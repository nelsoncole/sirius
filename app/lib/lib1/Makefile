CFLAGS=-Wall -O0 -m32 -ffreestanding -nostdlib -nostdinc -I ./include
.PHONY: lib1 clean move

lib1:
	gcc ${CFLAGS} -c sys/__main.c
	gcc ${CFLAGS} -c gui/*.c
	ar -rcs lib1.a *.o
	nasm -f elf32 -O0 sys/setup.asm
	gcc ${CFLAGS} -c sys/__main2.c
move:
	mv sys/*setup.o obj
	mv *.o obj

clean:
	rm *.o
