


#include <stdio.h>
#include <string.h>


FILE *fopen (const char *filename,const char *mode) 
{
	return 0;
}

int fclose (FILE *fp) 
{

	if(fp->header.mode[0] == '\0') return 0;

	if((fp->header.mode[0] == 'r') && (fp->header.mode[1] == 'x')) {


		fp->header.flag = 0;
		return EOF;


	}


	if ((fp->header.mode[0] == 's') \
	&& (fp->header.mode[1] == 't') && (fp->header.mode[2] == 'd')) {

		fp->header.flag = 0;
		//free_pages((void*)stream->header.buffer);
		//free(stream);


		return EOF;
	}


	return EOF; // successfull
}

int fflush(FILE *fp) {

	// error
	if(fp->header.mode[0] == '\0') return EOF;


	// rx
	if((fp->header.mode[0] == 'r') && (fp->header.mode[1] == 'x')) {

		return 0;


	}

	// std
	if ((fp->header.mode[0] == 's') \
	&& (fp->header.mode[1] == 't') && (fp->header.mode[2] == 'd')) {


		return 0;
	}


	
	return EOF;

}

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

		// write character
		*(unsigned char*)(buffer + offset) = ch;

		// Update offset
		offset = fp->header.offset +=1;

		return  (ch&0xff); // successfull
	}



	return EOF;
}

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
		putc(ch,stdout);

		return ch; // successfull
	}




	
	return (EOF);

}

size_t write (const void *buffer,size_t num_bytes, size_t count, FILE *fp)
{
	

	return 0;
}

size_t read (void *buffer,size_t num_bytes, size_t count, FILE *fp)
{
	return 0;
}

int feof (FILE *fp)
{

	return 0;
}

void rewind (FILE *fp)
{
	fp->header.offset = 0;

}

int fseek (FILE *fp, long num_bytes, int origin )
{

	switch(origin) {

	case SEEK_SET:
		break;
	case SEEK_CUR:
		break;
	case SEEK_END:
		break;

	}

	return 0;
}


int ferror (FILE *fp )
{
	return 0;
}

int remove (const char *filename)
{

	return 0;
}





// E/S pelo console
int putchar(int c)
{
	return (putc (c,stdout));

}

void puts(const char* str)
{
	
	int i;
	if(!str)return;
	 
	for(i=0;i <strlen(str);i++) putc (str[i],stdout);
 }








