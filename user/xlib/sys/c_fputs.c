#include <sys/sys.h>

#define EOF (-1)

int c_fputs (const char *str,FILE *fp)
{
	
	int rc;
	const char *p_str = str;

	while(*p_str != '\0') rc = c_putc (*p_str++,fp);
	

	return (rc);

}

