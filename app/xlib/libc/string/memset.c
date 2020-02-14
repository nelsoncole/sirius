#include <ctype.h>
#include <string.h>

void* memset(void *s, char val, size_t count)
{
	size_t _count = count;
        uint8_t *tmp = (uint8_t *)s;
        for(; _count != 0; _count--) *tmp++ = val;
        return s;    	
}
