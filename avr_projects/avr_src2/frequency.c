/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2007.  All rights reserved

 Version Date: 
 Wed Feb  9 12:21:24 EST 2011
-----------------------------------------------------------------
============================================================== */
#include "frequency.h"

unsigned int t1_prescale[5] = {1, 8, 64, 256, 1024};

void freq_init(void)
{
	//set_bit(DDRD, 6);
	//TCCR0A = (1<<COM0A0)|(2<<WGM00);
	set_bit(DDRD, 5);
	TCCR0A = (1<<COM0B0)|(2<<WGM00);
}

void freq_out(unsigned int frequency)
{
	int i = 0;
	int j = 0;

	/* Find best prescale match for frequency */
	do 
		j = (FOSC / ((unsigned long) frequency * 2 * t1_prescale[i++])); 
	while (j > 0xFF);

	OCR0A = j;

	/* Set prescale value */
	TCCR0B &= 0xF8;
	TCCR0B |= i;
}
