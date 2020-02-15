#include <string.h>

void* memset(void *s, char val, size_t count)
{
	size_t _count = count;
        unsigned char *tmp = (unsigned char *)s;
        for(; _count != 0; _count--) *tmp++ = val;
        return s;    	
}
