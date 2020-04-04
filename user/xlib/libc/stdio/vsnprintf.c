#include <stdio.h>
#include <string.h>


#define BUFFER_SIZE	65536

static char sn_buf[BUFFER_SIZE];

int vsnprintf(char * restrict s, size_t n,const char * restrict format, va_list arg)
{
	int rv;
	int bytes;

	if(n > BUFFER_SIZE ); // return error

	rv = vsprintf(sn_buf, format, arg);

	if(rv >= BUFFER_SIZE ); // "snprintf buffer overflow"


	bytes = rv;

	if (bytes > 0) {

		memcpy(s, sn_buf, bytes);
		s[bytes] = '\0';
    	}

	
    	return (rv);

}
