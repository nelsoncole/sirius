#include <sys/sys.h>
#include <stdio.h>


int fclose (FILE *fp) 
{
	return close_r(fp);
}

