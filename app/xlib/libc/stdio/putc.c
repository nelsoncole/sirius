#include <stdio.h>


int putc (int ch, FILE *fp) 
{
	unsigned char* buffer = (unsigned char*) (fp->header.buffer);
	//unsigned dev_n = stream->header.dev;
	//unsigned count = stream->header.count;
	unsigned int offset = fp->header.offset;
	//unsigned int total_blocks = stream->header.blocks;
	//unsigned int *block	= (unsigned int*)(stream->block);
	//unsigned int bps = stream->header.bps;
	//unsigned int lba_start;
	//unsigned int local_offset;
	
	

	// error
	if(fp->header.mode[0] == '\0') return EOF;

	

	// std
	if ((fp->header.mode[0] == 's') \
	&& (fp->header.mode[1] == 't') && (fp->header.mode[2] == 'd')) {
 


		if(offset >= 65536) {

			//setmem((unsigned char*)(buffer),65536,0);
			fp->header.offset = 0;
			fp->header.offset2 = 0;
			return EOF;

		}

		if(ch == '\b') {

			if(!fp->header.offset) return EOF;
			// Update offset
			offset = fp->header.offset -=1;

			// write character
			*(unsigned char*)(buffer + offset) = ' ';

		} else {

			// write character
			*(unsigned char*)(buffer + offset) = ch;

			// Update offset
			offset = fp->header.offset +=1;
		}

		return  (ch&0xff); // successfull
	}



	return EOF;
}
