#include <machinescience.h>
#include <twi_masterslave.cpp>
#include <ms_delay.cpp>


void setup_adc(void)
{
	ADMUX = 0xE; //Set the Band Gap voltage as the ADC input
	ADCSRA = (1<<ADEN)|(1<<ADATE)|(1<<ADIE)|(1<<ADSC)|5;
}

ISR(ADC_vect) //ADC End of Conversion interrupt 
{
	unsigned char adc_data;
	adc_data = ADC >> 2; //read 8 bit value
	//vcc = 1.1 * 255 / adc_data;
	vcc = 11 * 255 / adc_data;
}

int main(void)
{

	network_control(ENABLE);
	setup_adc(); //setup the ADC

	while(1) { // Infinite loop
		iomod_decimal(FIRST_LINE, vcc, 4);
	//	delay_ms(500);
	}
}
	
