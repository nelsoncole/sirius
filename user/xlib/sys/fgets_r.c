#include <sys/sys.h>

#define EOF (-1)
// Standard streams.
extern FILE *stdout;

char *std_fgets_r (char *str,int length,FILE *fp)
{
	int i,c;
	int count = 0;
	char *p_str = str;

	for (i=0;i < length-1;i++) {


		if(fp->level >= fp->bsize || fp->off >= fp->bsize) {

			fp->off = fp->off2 = 0;
			fp->level = 0;
		}

		if(fp->flags == 2/*stdin*/) {


			fp->size = 0;
			fp->off = fp->off2 = 0;
			fp->level = 0;


			while(!fp->off);
			while(!(fp->off - fp->off2 ));

				fp->curp = (unsigned char*)(fp->buffer + fp->off2);
				c = *(unsigned char*)(fp->curp) &0xff;

				if(c == '\n') {
					// display console
					putc_r(c,stdout);

					*p_str++ = c;
					break;

				}else if(c == '\b') {
 
					// display console
					if(count > 0) {
						*--p_str = '\b';
						putc_r(c,stdout);
						--count;
					}
	
					fp->off2++;
				} else {

					*p_str++ = c;
					// display console	
					putc_r(c,stdout);

					fp->off2++;
					count++;
				}
			}else if(fp->flags == 3/*stdout*/) {

				if(!fp->off)break;
				if((fp->off2 >= fp->off ))break;

				fp->curp = (unsigned char*)(fp->buffer + fp->off2);
				c = *(unsigned char*)(fp->curp) &0xff;

				if(c == '\n') {
					*p_str++ = c;
					fp->off2++;
					break;

				}else {
					*p_str++ = c;
					fp->off2++;

				}

			}else {	// stderr
					*p_str = '\0';
					return str;

			}

		}

		*p_str = '\0';
		return str; // successfull

}

char *fgets_r (char *str,int length,FILE *fp)
{
	int i, c;
	char *p_str = str;

	// error
	if(fp->mode[0] == '\0') return 0;

	

	// std
	if ((fp->mode[0] == 's') \
	&& (fp->mode[1] == 't') && (fp->mode[2] == 'd')) {

		return std_fgets_r (str,length,fp);

	}else if(TRUE /*outros*/){
	
		for(i=0;i < length-1;i++){

			c = getc_r (fp);
			if (c != '\n') *p_str++ = c;
			else {
				*p_str++ = c;
				break;
			}
	
		}



	}

	*p_str = '\0';

	return (str);
}
