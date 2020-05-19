#include <sys/sys.h>

#define EOF (-1)

int fputs_r (const char *str,FILE *fp)
{
	int rc;
	const char *p_str = str;

	while(*p_str != '\0') rc = putc_r (*p_str++,fp);
	

	return (rc);

}
