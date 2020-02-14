#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int printf(const char *format,...)
{
	char* ap;
	va_start (ap,format);
	int index = 0;
	unsigned char u;	
	int d;
	char c, *s;
	char buffer[256];

	while (format[index])
	{
		switch (format[index])
		{
		case '%':
			++index;
			switch (format[index])
			{
			
			case 'c':
				c = (char) va_arg (ap, int);
				putchar(c);
				break;

			case 's':
				s = va_arg (ap, char*);
				puts(s);
				break;

			case 'd':
			case 'i':
				d = va_arg (ap, int);
				strtol (buffer, (char **) NULL,d);
				puts(buffer);
				break;

			case 'u':
				u = va_arg (ap, uint32_t);
				strtol (buffer, (char **) NULL,u);
				puts(buffer);
				break;

			case 'X':
			case 'x':
				d = va_arg (ap, int);
				i2hex(d, buffer,8);
				puts(buffer);
				break;
			
			default:
				putchar('%');
				putchar('%');
				break;
				
				
			}
			break;

		default:
			putchar(format[index]); //
			break;
		}
		++index;
	}


	return 0;
}

