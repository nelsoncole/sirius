#include <stdlib.h>

int strtol (const char *nptr,char **endptr,int base) {
  	

	char* valuestring = (char*) nptr;
	int value = base;

	int min_flag;
  	char swap, *p;
 	min_flag = 0;

  	if (0 > value)
  	{
    		*valuestring++ = '-';
    		value = -INT_MAX> value ? min_flag = INT_MAX : -value;
  	}

  	p = valuestring;

  	do
  	{
    		*p++ = (char)(value % 10) + '0';
    		value /= 10;
  	} while (value);

  	if (min_flag != 0)
  	{
    		++*valuestring;
  	}
  	*p-- = '\0';

  	while (p > valuestring)
  	{
    		swap = *valuestring;
    		*valuestring++ = *p;
    		*p-- = swap;
  	}

	return base;
}
