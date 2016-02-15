/* -------------------------------------------------------------
 Machine Science API
 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Mon Jun 28 13:49:59 EDT 2010
 
-----------------------------------------------------------------
============================================================== */
#ifndef _TIMERS_H_
#define _TIMERS_H_

#define SLOW	0
#define FAST	1	

#include <gps_sensor.h>

extern volatile unsigned char gpsConvertState;
extern volatile unsigned char gpsState;

extern void interval_clock_init(char);
 
#endif
