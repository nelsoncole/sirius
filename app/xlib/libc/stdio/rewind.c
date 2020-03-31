#include <sys/sys.h>
#include <stdio.h>


void rewind (FILE *fp)
{
	if(!fp) return;

	if ((fp->header.mode[0] != 's') && \
	(fp->header.mode[1] != 't') && (fp->header.mode[2] != 'd')) 
	{ 

		if((fp->header.offset >= 0x10000) && (fp->header.flag&0x10)) 
		{
			flush(fp);
			fp->header.flag &=~0x10;
		}

	}


	fp->header.offset = fp->header.offset2 = 0;

}
