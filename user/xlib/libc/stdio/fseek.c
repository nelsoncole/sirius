#include <sys/sys.h>
#include <stdio.h>


int fseek (FILE *fp, long num_bytes, int origin )
{

	unsigned int offset;
	unsigned int new_offset;
	unsigned int var;

	if(!fp) return 0;

	if ((fp->mode[0] == 's') && \
	(fp->mode[1] == 't') && (fp->mode[2] == 'd')) 
	{ 
		switch(origin) {

			case SEEK_SET:
				fp->off = num_bytes;
				fp->off2 = num_bytes;
			break;
			case SEEK_CUR:
				fp->off += num_bytes;
				fp->off2 += num_bytes;
			break;
			case SEEK_END:
				fp->off = fp->size + num_bytes;
				fp->off2 = fp->off;
			break;

		}

		fp->level = fp->off % fp->bsize;

		return 0;

	}



	switch(origin) {

		case SEEK_SET:

			offset = fp->off/fp->bsize;
			new_offset = num_bytes/fp->bsize;

			if((offset != new_offset) && !(fp->flags & 0x10) ) 
			{	
				flush_r(fp);
				fp->flags |= 0x10;
			}

			fp->off = num_bytes;
			fp->off2 = num_bytes;
		break;
		case SEEK_CUR:
			offset = fp->off/fp->bsize;
			new_offset = (fp->off + num_bytes)/fp->bsize;

			if((offset != new_offset) && !(fp->flags & 0x10) ) 
			{	
				flush_r(fp);
				fp->flags |= 0x10;
			}

			fp->off += num_bytes;
			fp->off2 += num_bytes;
		break;
		case SEEK_END:
			offset = fp->off/fp->bsize;

			if(fp->size >= num_bytes) var = num_bytes;
			else var = 0;

			new_offset = (fp->size - var)/0x10000;

			if((offset != new_offset) && !(fp->flags & 0x10) ) 
			{	
				flush_r(fp);
				fp->flags = 0x10;
			}
			
			fp->off = fp->size - var;
			fp->off2 = fp->off;
		break;

	}

	fp->level = fp->off % fp->bsize;

	return 0;
}
