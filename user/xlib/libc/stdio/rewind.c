#include <sys/sys.h>
#include <stdio.h>


void rewind (FILE *fp)
{
	if(!fp) return;

	if ((fp->mode[0] != 's') && \
	(fp->mode[1] != 't') && (fp->mode[2] != 'd')) 
	{ 

		if((fp->off >= fp->bsize) && !(fp->flags & 0x10)) 
		{
			flush_r(fp);
			fp->flags |= 0x10;
		}

	}


	fp->off = fp->off2 = 0;
	fp->level = 0;

}
