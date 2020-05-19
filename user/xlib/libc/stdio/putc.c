
#include <sys/sys.h>
#include <stdio.h>


int putc (int ch, FILE *fp) 
{
	return putc_r (ch,fp);
}
