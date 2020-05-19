#include <sys/sys.h>
#include <stdio.h>

int fgetc (FILE *fp)
{	
	return getc_r (fp);

}

