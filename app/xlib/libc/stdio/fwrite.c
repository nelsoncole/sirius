#include <stdio.h>


size_t fwrite (const void *buffer,size_t num_bytes, size_t count, FILE *fp)
{
	return write (buffer,num_bytes,count,fp);
}
