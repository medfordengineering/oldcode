/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2007.  All rights reserved
 
 Version Date: Mon Nov 12 12:17:50 EST 2007
-----------------------------------------------------------------
============================================================== */
#include "interval.h"

void interval_init( unsigned int interval)
{
	/* This is the system heartbeat */
	timer0_interval(interval, 'A'); 		// Interval defined by MASTER_PULSE
	timer0_mode(TMR0_CTC);						// Top count is OCR0A
	timer0_interrupt(TMR0_MATCHA, ON);				// Interrupt at top count

}
