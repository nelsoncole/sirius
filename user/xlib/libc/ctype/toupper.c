#include <ctype.h>


// converte o caracter minúsculo em maiúsculo
int toupper(int c)
{
	if( (c > 0x60) && (c < 0x7b)) return (c - 0x20);
	
	return (c);

}
