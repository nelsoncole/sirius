#include <stdio.h>


void rewind (FILE *fp)
{
	fp->header.offset = fp->header.offset2 = 0;

}
