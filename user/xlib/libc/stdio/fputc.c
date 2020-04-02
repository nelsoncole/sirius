#include <sys/sys.h>
#include <stdio.h>


int fputc (int ch, FILE *fp) 
{
	return c_putc (ch,fp);
}
