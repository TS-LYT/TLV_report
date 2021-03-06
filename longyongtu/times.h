#ifndef _TIMES_H
#define _TIMES_H

#include <time.h>
#include <string.h>
#include "int_convert_string.h"

typedef struct tme {
	int tm_sec; /* seconds after the minute - [0,59] */
	int tm_min; /* minutes after the hour - [0,59] */
	int tm_hour; /* hours since midnight - [0,23] */
	int tm_mday; /* day of the month - [1,31] */
	int tm_mon; /* months since January - [0,11] */
	int tm_year; /* years since 1900 */
	int tm_wday; /* days since Sunday - [0,6] */
	int tm_yday; /* days since January 1 - [0,365] */
	int tm_isdst; /* daylight savings time flag */
}tm;
void print_time();
void get_time(char *gettime,int size);

#endif
