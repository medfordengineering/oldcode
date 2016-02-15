/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Fri Sep 25 12:52:41 EDT 2009
-----------------------------------------------------------------
============================================================== */

#ifndef _IRSENSE_H_
#define _IRSENSE_H_

#include <twi_masterslave.h>

/* Commands for ultra sensor */
#define CMD_IRSEN_READ				0x00
#define CMD_IRSEN_SETTWIADR 		0x01
#define CMD_IRSEN_DISPLAYTWIADR		0x02
#define CMD_IRSEN_WDSTATE			0x03
#define CMD_IRSEN_SETSTATE			0x04
#define CMD_IRSEN_FILTERCOUNT		0x05

#define AVERAGE_FILTER				0	
#define PASS_FILTER					0	

extern unsigned int read_distance(char );
extern void set_iraddress(char , char );
extern void display_iraddress(char , char );
extern void set_wdtstate(char , char );
extern void set_filtercount(char , char );
#endif
