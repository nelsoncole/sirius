#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#define HEX_LEN 8


static char *_str_i2hex(char *s, int del)
{
	unsigned char *us = (unsigned char *)s;
	while(*us) {  
		if(*us != del) return (char*)us;
		else us++;	
	}

	return (char*)--us;
}

void i2hex(unsigned int val, char* dest, int len)
{
	char* cp;
	char x;
	int n;
	n = val;
	cp = &dest[len];
	while (cp > dest)
	{
		x = n & 0xF;
		n >>= 4;
		*--cp = x + ((x > (HEX_LEN+1)) ? 'A' - 10 : '0');
	}

    	dest[len]='\0';

	cp = _str_i2hex(dest, '0');

	strcpy(dest,cp);

	n = cp - dest;

	if(n >= HEX_LEN ) n = HEX_LEN - 1;

	memset(dest + HEX_LEN - n,0,n);
}
