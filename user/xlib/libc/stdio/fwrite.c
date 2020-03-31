#include <sys/sys.h>
#include <stdio.h>


size_t fwrite (const void *buffer, size_t size, size_t count, FILE *fp)
{
	return _write (buffer,size,count,fp);
}
