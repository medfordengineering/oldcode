
/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2004.  All rights reserved
	
 Version Date: Fri Feb  8 17:51:34 EST 2008
-----------------------------------------------------------------
============================================================== */
#ifndef _UMP3_H
#define _UMP3_H

//#include "machinescience.h"
#include <ms.h>
#include "usart.h"
#include <ctype.h>

extern int uMP3_count_title(void);
extern void uMP3_read_title(char * title, char title_counter);
extern void uMP3_play( char * track);

#endif

