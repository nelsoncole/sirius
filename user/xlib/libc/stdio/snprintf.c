#include <stdio.h>

int snprintf(char * restrict s, size_t n,const char * restrict format, ...)
{

	va_list ap;
    	int rv;

    	va_start(ap, format);
    	rv = vsnprintf(s, n, format, ap);
    	va_end(ap);

    	return rv;

}
