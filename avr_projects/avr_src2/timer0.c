/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2007.  All rights reserved
 
 Version Date: Mon Nov 12 12:17:50 EST 2007
-----------------------------------------------------------------
============================================================== */
#include "timers.h"

//DOES NOT SEEM TO WORK WITH INTERVAL > 50000
//This also only sets prescale and OCR0..it assumes CTC or PWM with OCR0
//Maybe this should just call CTC mode
//And a separate one for PWM

void timer0_pwm_init(char mode, char prescale)
{
	timer0_mode(mode);
	timer0_prescale(prescale);
	timer0_output(TMR_CLR, 'A');
	timer0_output(TMR_CLR, 'B'); 
	timer0_duty(0, 'A');
	timer0_duty(0, 'B');
	set_bit(DDRD, 6);
	set_bit(DDRD, 5);
	//output_bit(DDRD, 6);
	//output_bit(DDRD, 5);
}

void timer0_interval(unsigned long interval_us, char channel)
{
	unsigned long temp;
	int count = 0;
	unsigned int prescale[5] = {0x01,0x08,0x40,0x100,0x400};

	do
		{
			temp = (FOSM * interval_us)/prescale[count++];
			
		} while (temp > 0xFF);

	if (channel == 'A')
		OCR0A = temp;
	else if (channel == 'B')
		OCR0B = temp;

	timer0_prescale(count);
}

void timer0_duty(unsigned char duty, unsigned char channel)
{
	if (channel == 'A')
//		OCR0A = duty * 0xff/100;
		OCR0A = duty;
	else if (channel == 'B')
		OCR0B = duty;
}

void timer0_mode(char mode)
{
	/* Sets mode as defined Table 13-8 */
	TCCR0A &= 0xF0; 			
	TCCR0A |= mode;
	TCCR0B &= 0xF7;
	TCCR0B |= ((mode << 1) & 0x08);
}

void timer0_prescale(int prescale)
{
	/* Sets prescale */
	TCCR0B &= 0xF8;
	TCCR0B |= prescale;
}

void timer0_output(char output, char channel)
{
	/* Sets output options for selected channel */
	if (channel == 'A'){
		TCCR0A &= 0x3F;
		TCCR0A |= (output << 6);
		}
	else if (channel == 'B'){
		TCCR0A &= 0xCF;
		TCCR0A |= (output << 4);
		}
}

void timer0_interrupt(char mode, char state)
{
	/* Sets options for interrupt */
	if (state == ON)
		set_bit(TIMSK0, mode);
	else if (state == OFF)
		clear_bit(TIMSK0, mode);
}

/*
void timer0_interrupt2(char match, char state)
{
	if (state == ON)
		set_bit(TIMSK0, match);
	else if (state == OFF)
		clear_bit(TIMSK0, match);
}
*/
