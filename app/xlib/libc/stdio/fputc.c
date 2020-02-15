#include <sys/sys.h>
#include <stdio.h>


int fputc (int ch, FILE *fp) 
{
	return _putc (ch,fp);
}
