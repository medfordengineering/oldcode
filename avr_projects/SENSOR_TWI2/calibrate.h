/* -------------------------------------------------------------
 Machine Science API
 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue May  5 16:29:15 EDT 2009
 
-----------------------------------------------------------------
============================================================== */
#ifndef _CALIBRATE_H_
#define _CALIBRATE_H_

#include <util/delay_basic.h>

/* (100 * FOSC) / 32768 */
#define OSCCAL_TARGET			24414 
/* 400 represents .25% accuracy */
#define OSCCAL_UPPER_LIMIT		OSCCAL_TARGET + (OSCCAL_TARGET / 400) 
#define OSCCAL_LOWER_LIMIT		OSCCAL_TARGET - (OSCCAL_TARGET / 400)

extern unsigned int osccal_calibrate( uint16_t, uint16_t); 
 
#endif
