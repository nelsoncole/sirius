
#include <sys/sys.h>
#include <stdio.h>


int putc (int ch, FILE *fp) 
{
	return c_putc (ch,fp);
}
