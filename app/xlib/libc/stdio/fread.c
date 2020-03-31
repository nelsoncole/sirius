#include <sys/sys.h>
#include <stdio.h>



size_t fread (void *buffer,size_t size, size_t count, FILE *fp)
{
	return _read (buffer,size,count,fp);
}
