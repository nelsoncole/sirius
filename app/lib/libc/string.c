#include <string.h>

size_t strlen(const char *s)
{
	char *tmp = (char*)s;
	
	while(*tmp != '\0')tmp++;

	return (size_t)(tmp - s);
}



void *memcpy(void *dest,void *src, size_t n)
{
	size_t p    = n;
	char *p_dest = (char*)dest;
	char *p_src  = (char*)src;

	while(p--)
	*p_dest++ = *p_src++;
	return dest;
}


void* memset(void *s, char val, size_t count)
{
        uint8_t *tmp = (uint8_t *)s;
        for(; count != 0; count--) *tmp++ = val;
        return s;    	
}

void* memsetw(void *s, short val, size_t count)
{	
        uint16_t *tmp = (uint16_t *)s;
        for( ;count != 0; count--) *tmp++ = val;
        return s;    	
}


char *strcpy(char *dest, const char *src)
{
    	char *p = (char*)dest;

	char *p_dest = (char*)dest;
	char *p_src  = (char*)src;

 
    	while (*p_src != '\0') *p_dest++ = *p_src++;
    
	return p;
}

int strncpy(char *dest, const char *src,size_t count)
{

	char *p_dest = (char*)dest;
	char *p_src  = (char*)src;

    	int i;
	for(i =0;i < count;i++) *p_dest++ = *p_src++;

	*p_dest ='\0';

    	return i;
}

int strcmp (char* s1, const char* s2)
{

    while((*s1++ == *s2++))
    if((*s1 == *s2) && (*s1 + *s2 == 0))
    return 0;


    return -1;
}
int strncmp (char* s1,const char* s2,size_t count)
{
	
	if(!count)
	return -1;

	while(--count)
	if(*s1++ != *s2++)
	break;

	return (count);
}
