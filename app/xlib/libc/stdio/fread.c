#include <stdio.h>



size_t fread (void *buffer,size_t num_bytes, size_t count, FILE *fp)
{
	return read (buffer,num_bytes,count,fp);
}
