/* -------------------------------------------------------------
 Machine Science API
 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   17 April 2007
-----------------------------------------------------------------
============================================================== */
#ifndef _CLOCK_H_
#define _CLOCK_H_

//#include "machinescience.h"
#include <ms.h>
#include "timers.h"

#define TIME_AM 0
#define TIME_PM 1

struct mstro_time {
	char seconds;
	char minutes;
	char hours;
	char ampm;
	char mode;
}time;

struct mstro_date{
	char day;
	char month;
	char year;
}date;
	
static const char daysOfMonths[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
extern void clock_init(void);
extern void time_set( char, char, char );
extern void date_set( char, char, char );

#endif
