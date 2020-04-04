#include <string.h>

size_t strcspn(const char *s1, const char *s2)
{
	size_t d = 0;

	const unsigned char *us1 = (const unsigned char *) s1;
	const unsigned char *us2 = (const unsigned char *) s2;

	for ( ;*us2 != 0 ;us2++) {

		us1 = (const unsigned char *) s1;
		d = 0;

		for ( ;*us1 != 0 ;us1++) {
			if(*us1 == *us2) return d;
			d++;
		}

	}

	return d;
}
