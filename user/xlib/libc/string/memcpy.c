#include <string.h>

void *memcpy(void *dest,void *src, size_t n)
{
	size_t p    = n;
	char *p_dest = (char*)dest;
	char *p_src  = (char*)src;

	while(p--)
	*p_dest++ = *p_src++;
	return dest;
}
