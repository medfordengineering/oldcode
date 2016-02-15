/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   10 January 2006
-----------------------------------------------------------------
============================================================== */

/* NOTES
a) 
*/

#ifndef _ONEWIRE_H_
#define _ONEWIRE_H_

#define F_CPU 20000000UL
#include <machinescience.h>
#include <util/delay.h>

/* ONEWIRE ROUTINES */

/* standard read, write and initialize routines for onewire */
/*
extern signed char onewire_reset(void);
extern void onewire_writebyte(char);
extern unsigned char onewire_readbyte(void);
*/
extern signed char onewire_reset(char);
extern void onewire_writebyte(char, char);
extern unsigned char onewire_readbyte(char);
#endif

