#include <stdio.h>
#include <stdlib.h>

int vfprintf(FILE *fp,const char * fmt, va_list ap)
{
	int index = 0;
	unsigned char u;	
	int d;
	char c, *s;
	char buffer[256];

	while (fmt[index])
	{
		switch (fmt[index])
		{
		case '%':
			++index;
			switch (fmt[index])
			{
			
			case 'c':
				c = (char) va_arg (ap, int);
				fputc(c,fp);
				break;

			case 's':
				s = va_arg (ap, char*);
				fputs(s,fp);
				break;

			case 'd':
			case 'i':
				d = va_arg (ap, int);
				itoa (d,buffer);
				fputs (buffer,fp);
				break;

			case 'u':
				u = va_arg (ap, unsigned int);
				itoa  (u,buffer);
				fputs(buffer,fp);
				break;

			case 'X':
			case 'x':
				d = va_arg (ap, int);
				i2hex(d, buffer,8);
				fputs(buffer,fp);
				break;
			
			default:
				fputs("%%",fp);
				break;
				
				
			}
			break;

		default:
			fputc(fmt[index],fp); //
			break;
		}
		++index;
	}

    	return (0);

}
