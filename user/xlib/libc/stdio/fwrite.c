#include <sys/sys.h>
#include <stdio.h>


size_t fwrite (const void *buffer, size_t size, size_t count, FILE *fp)
{
	return write_r (buffer,size,count,fp);
}
