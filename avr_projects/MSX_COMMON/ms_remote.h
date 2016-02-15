/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2004.  All rights reserved

 Version Date: Thu Dec  6 23:55:42 EST 2007
-----------------------------------------------------------------
============================================================== */

#ifndef _REMOTE_H_
#define _REMOTE_H_

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <machinescience.h>
#include <mxapi.h>
//#include <depco.h>

//USE 580 for 1us counter timer1

#define IR_PULSE 		72
#define IR_START		(IR_PULSE * 5)
#define IR_ZERO			(IR_PULSE * 2)
#define IR_ONE			(IR_PULSE * 3)
#define IR_OFFSET		(IR_PULSE / 2)

/* IR REMOTE CONTROL */
extern void remote_init(void);
extern unsigned char remote_read(void);

#endif

