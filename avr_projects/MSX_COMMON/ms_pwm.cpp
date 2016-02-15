/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2007.  All rights reserved
 
 Version Date: Mon Nov 12 12:17:50 EST 2007
-----------------------------------------------------------------
============================================================== */
#include <ms_pwm.h>

void pwm_init(void)
{
	/* Set timer0 pins to output */
	set_bit(DDRD, 6);
	set_bit(DDRD, 5);

	/* Set timer0 controls */
	TCCR0A = 	(1<<COM0A1)| 	// Clear OC0A on compare match, set OC0A at TOP
				(1<<COM0B1)| 	// Clear OC0B on compare match, set OC0B at TOP
				(1<<WGM01)| 	// Fast PWM
				(1<<WGM00);	

	/* Set prescale */
	TCCR0B &= 0xF8;
	TCCR0B |= 	(1<<CS01);		
	
	/* Initialize duty to 0 */
	OCR0A = 0;
	OCR0B = 0;
}

void pwm_duty(char channel, unsigned char duty)
{
	if (channel == 1)
		OCR0A = ((unsigned long) duty * 0xFF) / 100;
	else if (channel == 2)
		OCR0B = ((unsigned long) duty * 0xFF) / 100;
}

void pwm_freq(char divisor)
{
	TCCR0B &= 0xF8;
	TCCR0B |= divisor;
}

