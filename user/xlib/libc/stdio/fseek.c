#include <sys/sys.h>
#include <stdio.h>


int fseek (FILE *fp, long num_bytes, int origin )
{

	unsigned int offset;
	unsigned int new_offset;
	unsigned int var;

	if(!fp) return 0;

	if ((fp->header.mode[0] == 's') && \
	(fp->header.mode[1] == 't') && (fp->header.mode[2] == 'd')) 
	{ 
		switch(origin) {

			case SEEK_SET:
				fp->header.offset = num_bytes;
				fp->header.offset2 = num_bytes;
			break;
			case SEEK_CUR:
				fp->header.offset += num_bytes;
				fp->header.offset2 += num_bytes;
			break;
			case SEEK_END:
				fp->header.offset = fp->header.size + num_bytes;
				fp->header.offset2 = fp->header.offset;
			break;

		}

		return 0;

	}



	switch(origin) {

		case SEEK_SET:

			offset = fp->header.offset/0x10000;
			new_offset = num_bytes/0x10000;

			if((offset != new_offset) && (fp->header.flag&0x10) ) 
			{	
				flush(fp);
				fp->header.flag &=~0x10;
			}

			fp->header.offset = num_bytes;
			fp->header.offset2 = num_bytes;
		break;
		case SEEK_CUR:
			offset = fp->header.offset/0x10000;
			new_offset = (fp->header.offset + num_bytes)/0x10000;

			if((offset != new_offset) && (fp->header.flag&0x10) ) 
			{	
				flush(fp);
				fp->header.flag &=~0x10;
			}

			fp->header.offset += num_bytes;
			fp->header.offset2 += num_bytes;
		break;
		case SEEK_END:
			offset = fp->header.offset/0x10000;

			if(fp->header.size >= num_bytes) var = num_bytes;
			else var = 0;

			new_offset = (fp->header.size - var)/0x10000;

			if((offset != new_offset) && (fp->header.flag&0x10) ) 
			{	
				flush(fp);
				fp->header.flag &=~0x10;
			}
			
			fp->header.offset = fp->header.size - var;
			fp->header.offset2 = fp->header.offset;
		break;

	}

	return 0;
}
