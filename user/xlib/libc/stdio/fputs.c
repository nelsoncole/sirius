#include <sys/sys.h>
#include <stdio.h>


int fputs (const char *str,FILE *fp)
{
	return fputs_r (str,fp);

}

