/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2010.  All rights reserved
	
 Version Date: Fri Jul 30 14:03:31 EDT 2010
 -----------------------------------------------------------------
 ============================================================== */

#include <ms_analog.h>

char _resolution_bits;

/* Set the reference voltage */
void adc_reference(char reference)
{
	switch (reference){
		case ADC_AVCC:				//Internal VCC
			set_bit(ADMUX, REFS0);
			clear_bit(ADMUX, REFS1);
			break;
		case ADC_AREF:				//External AREF pin	
			clear_bit(ADMUX, REFS0);
			clear_bit(ADMUX, REFS1);
			break;
		case ADC_11V:				//Interal 1.1V reference
			set_bit(ADMUX, REFS0);
			set_bit(ADMUX, REFS1);
			break;
	}
}

/* Set 8 or 10 bit resolution */
void adc_resolution(char resolution)
{
	if (resolution == ADC_8BIT) {
		set_bit(ADMUX, ADLAR);
		_resolution_bits = 1;
	}
	else if (resolution == ADC_10BIT) {
		clear_bit(ADMUX, ADLAR);
		_resolution_bits = 0;
	}
}

/* Set the prescale */
void adc_prescale(char prescale)
{
	ADCSRA &= 0xF8; 
	ADCSRA |= prescale;
}

/* Read the channel on breadboard */
unsigned int adc_read(char channel)
{
	//const char masterPortConvert[6] = {6,7,0,1,2,3};		

	/* Set analog channel */
	ADMUX &= 0xF0;
	ADMUX |= (int)channel;

	/* Start analog conversion */
	set_bit(ADCSRA, ADSC);

	/* Wait for analog converstion to complete */
	while (test_bit(ADCSRA, ADSC));
	
	if (_resolution_bits == 0)
		return ADCW;
	else 
		return ADCH;
}

/* Read the channel */
unsigned int analog_read(char channel)
{
	const char masterPortConvert[6] = {7,6,3,2,1,0};		

	/* Set analog channel */
	ADMUX &= 0xF0;
	ADMUX |= masterPortConvert[(int)channel];

	/* Start analog conversion */
	set_bit(ADCSRA, ADSC);

	/* Wait for analog converstion to complete */
	while (test_bit(ADCSRA, ADSC));
	
	if (_resolution_bits == 0)
		return ADCW;
	else 
		return ADCH;
}

/* Read channel and average over 1000 */
unsigned int adc_average(char channel)
{
	unsigned long analogValue = 0;
	unsigned int i;

	/* Sum the reading samples. */
	for (i = 0; i < 1000; i++)
		analogValue += adc_read(channel);
	
	/* Return averaged value. */
	return analogValue /= 1000;
}

/* Back compatable with breadboards */
void adc_init(void)
{
	adc_control(ENABLE);
}

/* Default initialization for analog control unit */
void adc_control(unsigned char state)
{
	adc_reference(ADC_AVCC);
	adc_resolution(ADC_10BIT);
	adc_prescale(0x06);
	if (state == ENABLE) 
		set_bit(ADCSRA, ADEN);
	else
		clear_bit(ADCSRA, ADEN);
}


