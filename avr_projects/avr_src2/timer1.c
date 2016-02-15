/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2007.  All rights reserved
 
 Version Date: Mon Nov 12 12:17:50 EST 2007
-----------------------------------------------------------------
============================================================== */
#include "timers.h"

void timer1_interval(unsigned long interval_us, char channel)
{
	unsigned int temp;
	int count = 0;
	unsigned int prescale[5] = {0x01,0x08,0x40,0x100,0x400};

	do
		{
			temp = (FOSM * interval_us)/prescale[count++];
			
		} while (temp > 0xFFFF);

	if (channel == 'A')
		OCR1A = temp;
	else if (channel == 'B')
		OCR1B = temp;

	timer1_prescale(count);
}

void timer1_mode(char mode)
{
	/* Sets mode as defined Table 13-8 */
	TCCR1A &= 0xF0; 			
	TCCR1A |= mode;
	TCCR1B &= 0xC7;
	TCCR1B |= ((mode << 1) & 0x18);
}

void timer1_prescale(int prescale)
{
	/* Sets prescale */
	TCCR1B &= 0xF8;
	TCCR1B |= prescale;
}

void timer1_output(char output, char channel)
{
	/* Sets output options for selected channel */
	if (channel == 'A'){
		TCCR1A &= 0x3F;
		TCCR1A |= (output << 6);
		}
	else if (channel == 'B'){
		TCCR1A &= 0xCF;
		TCCR1A |= (output << 4);
		}
}

void timer1_interrupt(char mode, char state)
{
	if (state == ON)
		set_bit(TIMSK1, mode);
	else if (state == OFF)
		clear_bit(TIMSK1, mode);
}

