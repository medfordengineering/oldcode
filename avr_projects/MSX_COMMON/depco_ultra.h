/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Fri Sep 25 12:52:41 EDT 2009
-----------------------------------------------------------------
============================================================== */

#ifndef _ULTRASENSE_H_
#define _ULTRASENSE_H_

#include <twi_masterslave.h>

/* Commands for ultra sensor */
#define CMD_ULTRA_CONVERT		0x00
#define CMD_SET_TIMEOUTDELAY	0x01
#define CMD_SET_RECHARGEDELAY	0x02
#define CMD_SET_PULSETRANSMIT	0x03
#define CMD_ULTRA_READ			0x04
#define CMD_ULTRA_WDTSTATE		0x05
#define CMD_ULTRA_SETSTATE		0x06
#define CMD_ULTRA_SETTWIADR		0x07
#define CMD_ULTRA_DISPLAYTWIADR	0x08

#define INCHES					0
#define TENTH_INCHES			1
#define CENTIMETERS				2
#define MILIMETERS				3
#define RAW						4

#define AUTO					0
#define MANUAL					1

extern unsigned int read_ultrasonic(char , char );
extern void set_state_ultrasonic(unsigned char , char );
extern unsigned int one_read_ultrasonic(char , char );
extern void set_ultraaddress(char , char );
extern void display_ultraaddress(char , char );

#endif
