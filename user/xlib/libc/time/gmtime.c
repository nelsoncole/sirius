#include <time.h>

struct tm _tm_r[1];

struct tm *gmtime(const time_t *timer)
{

	return (struct tm*) _tm_r;	


}
