/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   18 Feburary 2012
-----------------------------------------------------------------
============================================================== */

#ifndef _MS_DELAY_H_
#define _MS_DELAY_H_

#include <machinescience.h>

#define nop() 					asm volatile("nop\n")

#if FOSC == 1000000L
	#define _pause_nop
#elif FOSC == 4000000L
	#define _pause_nop 
#elif FOSC == 8000000L
	#define _pause_nop "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
#elif FOSC == 10000000L
	#define _pause_nop "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
#elif FOSC == 16000000L
	#define _pause_nop "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n" 
#elif FOSC == 20000000L
	#define _pause_nop "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n"
#endif

extern void delay_us (unsigned int cnt);
 
extern void delay_ms (unsigned int ms);

#endif

