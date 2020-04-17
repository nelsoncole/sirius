#include <stdlib.h>

#undef        getenv
char str[] = "\0(null)";
char str2[256];

char *getenv(const char *name)
{			

	return ((char*)str);


}
