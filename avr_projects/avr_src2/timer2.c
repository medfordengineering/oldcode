/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2007.  All rights reserved
 
 Version Date: Mon Nov 12 12:17:50 EST 2007
-----------------------------------------------------------------
============================================================== */
#include "timers.h"

void timer2_interval(unsigned long interval_us, char channel)
{
	unsigned int temp;
	int count = 0;
	unsigned int prescale[5] = {0x01,0x08,0x40,0x100,0x400};

	do
		{
			temp = (FOSM * interval_us)/prescale[count++];
			
		} while (temp > 0xFF);

	if (channel == 'A')
		OCR2A = temp;
	else if (channel == 'B')
		OCR2B = temp;

	timer2_prescale(count);
}

void timer2_duty(unsigned char duty, unsigned char channel)
{
	if (channel == 'A')
		OCR2A = duty * 0xff/100;
	else if (channel == 'B')
		OCR2B = duty * 0xff/100;
}

void timer2_mode(char mode)
{
	/* Sets mode as defined Table 13-8 */
	TCCR2A &= 0xF0; 			
	TCCR2A |= mode;
	TCCR2B &= 0xC7;
	TCCR2B |= ((mode << 1) & 0x18);
}

void timer2_prescale(int prescale)
{
	/* Sets prescale */
	TCCR2B &= 0xF8;
	TCCR2B |= prescale;
}

void timer2_output(char output, char channel)
{
	/* Sets output options for selected channel */
	if (channel == 'A'){
		TCCR2A &= 0x3F;
		TCCR2A |= (output << 6);
		}
	else if (channel == 'B'){
		TCCR2A &= 0xCF;
		TCCR2A |= (output << 4);
		}
}

void timer2_interrupt(char mode)
{
	/* Sets options for interrupt */
	TIMSK2 = mode;
}


