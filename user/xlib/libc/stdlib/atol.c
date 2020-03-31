
#include <stdlib.h>


#undef        atol


//Convert a string to an int.

int atol (const char *nptr)
{
  return (int) strtol (nptr, (char **) NULL, 10);
}
