#include <sys/sys.h>
#include <stdio.h>


char *fgets (char *str,int length,FILE *fp)
{
	return fgets_r (str,length,fp);

}

