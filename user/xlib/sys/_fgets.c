#include <sys/sys.h>

#define EOF (-1)
// Standard streams.
extern FILE *stdout;

char *_fgets (char *str,int length,FILE *fp)
{
	unsigned char* buffer = (unsigned char*) (fp->header.buffer);
	int i, ch;
	int count = 0;
	char *p_str = str;

	// error
	if(fp->header.mode[0] == '\0') return 0;

	

	// std
	if ((fp->header.mode[0] == 's') \
	&& (fp->header.mode[1] == 't') && (fp->header.mode[2] == 'd')) {

		for (i=0;i < length-1;i++) {


			if(fp->header.offset >= 65536) {
				fp->header.offset  = 0;
				fp->header.offset2 = 0;

			}

			if(fp->header.attr == 2/*stdin*/) {


				fp->header.size = 0;
				fp->header.offset = 0;
				fp->header.offset2 = 0;


				while(!fp->header.offset);
				while(!(fp->header.offset - fp->header.offset2 ));

				ch = *(unsigned char*)(buffer + fp->header.offset2);

				if(ch == '\n') {
					// display console
					_putc(ch,stdout);

					*p_str++ = ch;
					break;

				}else if(ch == '\b') {
 
					// display console
					if(count > 0) {
						*--p_str = '\b';
						_putc(ch,stdout);
						--count;
					}
	
					fp->header.offset2++;
				} else {

					*p_str++ = ch;
					// display console	
					_putc(ch,stdout);

					fp->header.offset2++;
					count++;
				}
			}else if(fp->header.attr == 3/*stdout*/) {

				if(!fp->header.offset)break;
				if((fp->header.offset2 >= fp->header.offset ))break;

				ch = *(unsigned char*)(buffer + fp->header.offset2);

				if(ch == '\n') {
					*p_str++ = ch;
					fp->header.offset2++;
					break;

				}else {
					*p_str++ = ch;
					fp->header.offset2++;

				}

			}else {	// stderr
					*p_str = '\0';
					return str;

			}

		}

		*p_str = '\0';
		return str; // successfull

	}else if(TRUE /*outros*/){
	
		for(i=0;i < length-1;i++){

			ch = _getc (fp);
			if (ch != '\n') *p_str++ = ch;
			else {
				*p_str++ = ch;
				break;
			}
	
		}



	}

	*p_str = '\0';

	return (str);


}

