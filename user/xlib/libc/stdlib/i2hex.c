#include <stdlib.h>

#define HEX_LEN 8

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
}
