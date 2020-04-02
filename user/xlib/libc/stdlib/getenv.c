#include <stdlib.h>

#undef        getenv
char str[] = "(null)";
char str2[256];

char *getenv(const char *name)
{			

	return ((char*)str);


}
