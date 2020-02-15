
#include <sys/sys.h>
#include <stdio.h>


int putc (int ch, FILE *fp) 
{
	return _putc (ch,fp);
}
