#include <ms.h>
#include <adc.c>
//#include <usart.c>
//#include <ms_print.c>
//#include <usart_print.c>
//#include <delay.c>

volatile unsigned char MasterCounter;

ISR (TIMER0_COMPA_vect)
{
	OCR0A = TCNT0 + MasterCounter;
}

void flash(void)
{
	//Toggle OC0A
	TCCR0A = (1<<COM0A0) | (1<<WGM01);
	//Set 1025 prescale
	TCCR0B = (5<<CS00);
	//Set count
	OCR0A = 250;

}

void pwm_led(void)
{
	//Set OC0A as an output
	DDRD = (1<<DDD6);
	//Set PORTD6 as high;
	PORTD = (1<<PD6);
	//Output on D6 and phase correct PWM 
	TCCR0A = (2<<COM0A0) | (1<<WGM00);
	//Prescale 8
	TCCR0B = (5<<CS00);
	//Duty = 0
	OCR0A = 0;
}

void measure_voltage (void)
{
	//Internal 1.1V reference, 8-bit 
	ADMUX =  (1<<ADLAR) | (0xE<<MUX0);
	//Set prescale to 128 and enable ADC
	ADCSRA = (1<<ADEN) | (7<<ADPS0);
}

int main(void)
{
	unsigned char supply;

	//usart_init();
	//UBRR0H = 0;
	//UBRR0L = 12;
//	adc_init();
//	adc_resolution(ADC_8BIT);
	pwm_led();	
	OCR0A = 128;
	sei();
	while(1);
	while(1) {
		OCR0A = adc_read(5);
	}

	while(1) {
		set_bit(ADCSRA, ADSC);
		while(test_bit(ADCSRA, ADSC));
		supply = ADCH;
		if (supply > 85)
			flash();
		else {
			TCCR0A = 0;
			PORTD = (1<<PD6);
		}
		//usart_decimal(supply, 4);
		//usart_return();
	}
}


/*
setup_adc(void)
{
	    ADMUX = 0xE; //Set the Band Gap voltage as the ADC input
		    ADCSRA = (1<<ADEN)|(1<<ADATE)|(1<<ADIE)|(1<<ADSC)|5;
}

ISR(ADC_vect) //ADC End of Conversion interrupt 
{
	unsigned char adc_data;
	adc_data = ADC>>2; //read 8 bit value
	vcc = 1.1 * 255 / adc_data;
}
*/
