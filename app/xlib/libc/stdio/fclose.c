#include <stdio.h>


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

