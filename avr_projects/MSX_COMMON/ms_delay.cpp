/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   18 Feburary 2006
-----------------------------------------------------------------
============================================================== */

#include "ms_delay.h"

void delay_us (unsigned int cnt)
{
		if (!cnt) return;
		 
        __asm__ volatile ( 
        						"\n\t" 
        "rjmp L_dl2%="    		"\n"  		/* skip 12 nops initially (see comment above) */ 
     	"L_dl1%=:"				"\n\t"     	/* almost one microsecond worth of NOPs follows .. */ 
      //  "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
	  	_pause_nop
     	"L_dl2%=:"       		"\n\t"    	/* enter here to skip 12 of the nops */ 
        "sbiw %0,1"   			"\n\t"    	/* 2-clock instruction */       
        "brne L_dl1%="  		"\n\t"    	/* usually 2-clock instruction */ 
         ::"w" (cnt)); 
} 
 
void delay_ms (unsigned int ms)
{
	if (ms < 1)	return;
	
	{
     unsigned int cnt;
     asm volatile (
         "\n"
         "L_dl1%=:"			"\n\t"
         	"ldi %A0,%2"	"\n\t"	/* set up inner loop count */
         	"ldi %B0,%3"	"\n"
         "L_dl2%=:"			"\n\t"	/* start a two-instruction, 4-clock inner loop */
         	"sbiw %A0, 1"	"\n\t"
         	"brne L_dl2%="	"\n\t"
         	"sbiw %1,1"		"\n\t"	/* count one millisecond */
         	"brne L_dl1%="	"\n\t"
         : "=&w" (cnt)
         : "w" (ms),
         	"M" ((unsigned int)(FOSC/4000) & 0xFF),
         	"M" ((unsigned int)(FOSC/4000) >> 8)
         );
	}
} 



