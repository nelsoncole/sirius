#include <ctype.h>


// converte o caracter maiúsculo em minúsculo
int tolower(int c)
{

	if( (c > 0x40) && (c < 0x60)) return (c + 0x20);
	
	return (c);

}
