#include <analog.h>

void adc_init(void)
{
	/* Set the reference voltage */
	set_bit(ADMUX, REFS0);
	clear_bit(ADMUX, REFS1);

	/* Set resolution to 10 bit */
	clear_bit(ADMUX, ADLAR);

	/* Enable analog converter */
	set_bit(ADCSRA, ADEN);

	/* Prescale of 64 for 8mHz clock and 200khz adc */
	ADCSRA |= 0x06;		

}

unsigned int adc_read(char channel)
{
	/* Set analog channel */
	ADMUX &= 0xF0;
	ADMUX |= channel;

	/* Start analog conversion */
	set_bit(ADCSRA, ADSC);

	/* Wait for analog converstion to complete */
	while (test_bit(ADCSRA, ADSC));
	
	return ADCW;
}


