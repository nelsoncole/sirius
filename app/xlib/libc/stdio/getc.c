#include <stdio.h>



int getc (FILE *fp)
{
	unsigned char* buffer = (unsigned char*) (fp->header.buffer);
	//unsigned dev_n = stream->header.dev;
	//unsigned count = stream->header.count;
	//unsigned int offset = fp->header.offset;
	//unsigned int offset2 = fp->header.offset2;
	//unsigned int total_blocks = stream->header.blocks;
	//unsigned int *block	= (unsigned int*)(stream->block);
	//unsigned int bps = stream->header.bps;
	//unsigned int lba_start;
	//unsigned int local_offset;
	int ch;
	
	
	// error
	if(fp->header.mode[0] == '\0') return EOF;

	

	// std
	if ((fp->header.mode[0] == 's') \
	&& (fp->header.mode[1] == 't') && (fp->header.mode[2] == 'd')) {

		fp->header.offset = 0;
		fp->header.offset2 = 0;

		while(!fp->header.offset);

		ch = *(unsigned char*)(buffer + fp->header.offset2);

		// display console	
		if(ch != '\b')putc(ch,stdout);

		return ch; // successfull
	}




	
	return (EOF);

}

