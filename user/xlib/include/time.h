#ifndef __TIME_H
#define __TIME_H

#include "size_t.h"

#define NULL ((void *)0)

#define CLOCKS_PER_SEC 0

typedef signed 	time_t;
typedef int	clock_t;

struct tm {
	int tm_sec;         /* seconds,  range 0 to 59          */
   	int tm_min;         /* minutes, range 0 to 59           */
   	int tm_hour;        /* hours, range 0 to 23             */
   	int tm_mday;        /* day of the month, range 1 to 31  */
   	int tm_mon;         /* month, range 0 to 11             */
   	int tm_year;        /* The number of years since 1900   */
   	int tm_wday;        /* day of the week, range 0 to 6    */
   	int tm_yday;        /* day in the year, range 0 to 365  */
   	int tm_isdst;       /* daylight saving time             */
};



#endif
