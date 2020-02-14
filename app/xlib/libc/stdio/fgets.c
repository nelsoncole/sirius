#include <stdio.h>


char *fgets (char *str,FILE *fp)
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
	char *p_str = str;
	int count = 0;
	
	// error
	if(fp->header.mode[0] == '\0') return 0;

	

	// std
	if ((fp->header.mode[0] == 's') \
	&& (fp->header.mode[1] == 't') && (fp->header.mode[2] == 'd')) {

		fp->header.offset = 0;
		fp->header.offset2 = 0;

		
		for (;;) {


			if(fp->header.offset >= 65536) {

				fp->header.offset  = 0;
				fp->header.offset2 = 0;

			}



			while(!fp->header.offset);
			while(!(fp->header.offset - fp->header.offset2));

			ch = *(unsigned char*)(buffer + fp->header.offset2);

			if(ch == '\n') {
				// display console
				putc(ch,stdout);

				*p_str = '\0';

				return str;

			}else if(ch == '\b') {
 
				// display console
				if(count > 0) {

					*--p_str = '\0';

					putc(ch,stdout);
					--count;
				}
	
				fp->header.offset2++;
	
				

			} else {

				*p_str++ = ch;
				// display console	
				putc(ch,stdout);

				fp->header.offset2++;
				count++;
			}

		}

		return str; // successfull
	}




	
	return (0); //error

}

