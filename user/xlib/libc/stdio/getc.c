#include <sys/sys.h>
#include <stdio.h>


int getc (FILE *fp)
{	
	return getc_r (fp);

}

