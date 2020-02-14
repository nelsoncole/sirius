#include <stdio.h>

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
