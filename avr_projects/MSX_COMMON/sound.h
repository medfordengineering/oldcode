/* -------------------------------------------------------------
 Machine Science API
 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   Wed Nov  5 14:41:07 EST 2008
-----------------------------------------------------------------
============================================================== */
#ifndef _TONE_H_
#define _TONE_H_

#include "ms_delay.h"
#include "ms_pins.h"

#define C4N						262
#define C4S						277
#define D4F						277
#define D4N						294
#define D4S						311
#define E4F						311
#define E4N						330
#define F4N						349
#define F4S						370
#define G4F						370
#define G4N						392
#define G4S						415
#define A4F						415
#define A4N						440
#define A4S						466
#define B4F						466
#define B4N						494
#define C5N						523
#define C5S						554
#define D5F						554
#define D5N						587
#define D5S						622
#define E5F						622
#define E5N						659
#define F5N						698
#define F5S						740
#define G5F						740
#define G5N						784
#define G5S						830
#define A5F						830
#define A5N						880
#define A5S						932
#define B5F 					932
#define B5N						988
#define C6N						1047
#define C6S						1109
#define D6F						1109
#define D6N						1175
#define D6S						1245
#define E6F						1245
#define E6N						1319
#define F6N						1397
#define F6S						1480
#define G6F						1480
#define G6N						1568
#define G6S						1661
#define A6F						1661
#define A6N						1760
#define A6S						1865
#define B6F						1865
#define B6N						1975

#define No64					25	
#define No32					No64 * 2
#define Nd32					No64 * 3
#define No16					No64 * 4
#define Nd16					No64 * 6
#define No08					No64 * 8
#define Nd08					No64 * 12 
#define No04					No64 * 16
#define Nd04					No64 * 24
#define No02					No64 * 32
#define Nd02					No64 * 48
#define No01					No64 * 64 
#define Nd01					No64 * 96


extern void tone_out(unsigned char, int, int);
	
#endif
