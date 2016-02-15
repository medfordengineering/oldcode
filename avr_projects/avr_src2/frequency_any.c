/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2007.  All rights reserved

 Version Date: Mon Nov 19 08:54:59 EST 2007
-----------------------------------------------------------------
============================================================== */
#include "frequency.h"

unsigned int _timer1_prescale;

unsigned int _timer1_pre_values[5] = {1, 8, 64, 256, 1024};

void frequency_prescale(unsigned char);
void frequency_channel(unsigned char);

void frequency_channel(unsigned char channel)
{
	if (channel == 1) {

		/* SET OCR0A to output */
		set_bit(DDRD, 6);							
		
		/* SET T0 FOR TOGGLE PIN OCR0A */
		set_bit(TCCR0A, COM0A0);

		/* Interrupt on match OCR0A */
		set_bit(TIMSK0, OCIE0A);

	}
	else if (channel == 2) {

		/* SET OCR0B to output */
		set_bit(DDRD, 5);							
		
		/* SET T1 FOR TOGGLE PIN OCR0B */
		set_bit(TCCR0A, COM0B0);

		/* Interrupt on match OCR0B */
//		set_bit(TIMSK0, OCIE0B);
	}
}

/* THIS SHOULD BE TIMER ONE FUNCTION */
void frequency_prescale(unsigned char scale)
{
	TCCR0B &= 0xF8;
	TCCR0B |= scale;
	_timer1_prescale = _timer1_pre_values[scale - 1];
}

/* Interrupt on TCNT0 match compare with OCR0A */
ISR (TIMER0_COMPA_vect)
{
	toggle_bit(PORTD, 1);

}

/* Interrupt on TCNT0 match compare with OCR0B */
ISR (TIMER0_COMPB_vect)
{
	OCR0B += _frequency[1];
}

void freq_out(unsigned char channel, unsigned int frequency)
{
	/* Set frequency of beep */
	if (channel == 1)
//		_frequency[0] = FOSC / ((unsigned long) frequency * 2 * _timer1_prescale);
		OCR0A = FOSC / ((unsigned long) frequency * 2 * _timer1_prescale);
	else if (channel == 2)
		//_frequency[1] = FOSC / ((unsigned long) frequency * 2 * _timer1_prescale);
		OCR0B = FOSC / ((unsigned long) frequency * 2 * _timer1_prescale);
}

void freq_init(char channel)
{
	TCCR0A |= (2<<WGM00);
	frequency_channel(channel);
	frequency_prescale(3);
	sei();
}
