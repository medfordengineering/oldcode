/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2010.  All rights reserved

Version Date: Fri Feb 12 16:48:12 EST 2010
-----------------------------------------------------------------
============================================================== */

#include <mastermod.h>

void digital_port(int8_t port, int8_t direction)
{
	if (direction == OUTPUT) 
		set_bit(DDRB, port);
	else if (direction == INPUT) {
		clear_bit(DDRB, port);
		set_bit(PORTB, port); 		//Set pull-ups on input pins
	}
}

void analog_input(unsigned char state)
{
	/* Set ADC for ADVCC or supply voltage reference and set for
	 * 10 bit resolution */
	ADMUX = (1<<REFS0) | (0<<REFS1) | (0<<ADLAR);

	/* Enable ADC and set clock to 62.5kHz */
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);	
}

unsigned int adc_convert(char channel)
{
	/* Select ADC channel */
	ADMUX &= 0xF0;	
	ADMUX |= channel;

	/* Start ADC conversion */
	set_bit(ADCSRA, ADSC);

	/* Wait for conversion to complete */
	while (test_bit(ADCSRA, ADSC));

	/* Return low and high ADC value combined */
	return ADCW;
}

unsigned int analog_read(int port)
{
	unsigned long analogValue = 0;
	unsigned int i;
	const char masterPortConvert[6] = {7,6,3,2,1,0};		

	/* Sum the reading samples. */
	for (i = 0; i < 1000; i++)
		analogValue += adc_convert(masterPortConvert[port]);
	
	/* Return averaged value. */
	return analogValue /= 1000;
}

