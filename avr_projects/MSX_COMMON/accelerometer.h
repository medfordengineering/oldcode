
/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2012.  All rights reserved

Version Date: Thu Feb  9 14:19:47 EST 2012
-----------------------------------------------------------------
============================================================== */
#ifndef _ACCELEROMETER_H_
#define _ACCELEROMETER_H_

#include <twi_masterslave.h>

#define ACCELEROMETER_MODULE 0x1D
#define XAXIS	1
#define YAXIS	2
#define ZAXIS	3
extern void xcell_setdata(unsigned char , unsigned char );
extern void xcell_getdata(char , signed int * );
extern void accelerometer_init(void);
extern signed int accelerometer_read(unsigned char );

#endif
