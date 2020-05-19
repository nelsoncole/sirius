
#include <sys/sys.h>
#include <stdio.h>


long int ftell(FILE *fp)
{
	if(!fp) return (0);

	return (fp->off);

}
