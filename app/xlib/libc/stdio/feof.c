#include <stdio.h>



int feof (FILE *fp)
{
	if(!fp) return EOF;
	if(fp->header.offset >= fp->header.size) return EOF;

	return 0;
}
