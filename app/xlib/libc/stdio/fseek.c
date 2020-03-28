#include <stdio.h>


int fseek (FILE *fp, long num_bytes, int origin )
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
