#include <sys/sys.h>
#include <stdio.h>
#include <string.h>

#define EOF (-1)


int setkey() {
	FILE *fp = stdin;
	fp->off2 = fp->off = 0;
	
	fp->size = 0;

	return fp->off2;
}

int getckey ()
{

	FILE *fp = stdin;

	if(!fp) return EOF;

	unsigned char* buffer = (unsigned char*) (fp->buffer);

	int ch;

	if(fp->off2  >= fp->off) return EOF;

	ch = *(unsigned char*)(buffer + fp->off2);

	fp->off2 += 1;

	return (ch&0xff); // successfull
}

