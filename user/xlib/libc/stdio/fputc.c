#include <sys/sys.h>
#include <stdio.h>


int fputc (int ch, FILE *fp) 
{
	return putc_r (ch,fp);
}
