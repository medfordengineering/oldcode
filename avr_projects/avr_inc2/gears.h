/* -------------------------------------------------------------
 Machine Science API
 (c) Copyright Machine Science, 2006.  All rights reserved

Version Date: Thu Mar  5 15:29:51 EST 2009
-----------------------------------------------------------------
============================================================== */
#ifndef _GEARS_H_
#define _GEARS_H_

#include <machinescience.h>
#include <heartbeat.h>
#include <adc.h>
#include <pins.h>

#define FORWARD				1
#define REVERSE 			0
#define STOP				2
#define SPIN_LEFT			4
#define SPIN_RIGHT			3
	
#define CLOCKWISE			1
#define COUNTERCLOCKWISE	0

#define digital pin_value

unsigned char intMotCount;
//unsigned char motorPD[6] = {15,15,15,15,15,15};
unsigned char motorPD[6];
extern void gears_init(void);
extern void gears_motor(int, char, char);
extern void gears_move(char, char);
extern unsigned int analog(int);

#endif 
