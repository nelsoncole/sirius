#include <io.h>
char *utf8_convert(char *str,int *unicode)
{
	unsigned char *ustr = (unsigned char *)str;
	
	int c = *ustr++ &0xff;
	if(c > 191 && c < 224) {
		// caracter de 2 bytes
		c = (c &0xf0) + (*ustr++ &0x7f);
	}else {
		// caracter de 1 bytes
	}

	*(int*)unicode = c;

	return (char *)ustr;
}
