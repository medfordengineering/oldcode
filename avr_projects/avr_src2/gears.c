/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

Version Date: Thu Mar  5 15:28:28 EST 2009
-----------------------------------------------------------------
============================================================== */

#include <gears.h>

ISR (TIMER0_COMPA_vect)
{
	intMotCount++;
	if (intMotCount == motorPD[0])
		clear_bit(PORTD, 2);
	if (intMotCount == motorPD[1])
		clear_bit(PORTD, 3);
	if (intMotCount == motorPD[2])
		clear_bit(PORTD, 4);
	if (intMotCount == motorPD[3])
		clear_bit(PORTD, 5);
	if (intMotCount == motorPD[4])
		clear_bit(PORTD, 6);
	if (intMotCount == motorPD[5])
		clear_bit(PORTD, 7);
	if (intMotCount == 200){
		PORTD |= 0xfc;
		intMotCount = 0;
	}
}

void gears_motor(int motor, char direction, char speed)
{
	if (direction == FORWARD) 
		speed += 15;
	if (direction == REVERSE)
		speed = 15 - speed;

	motorPD[motor] = speed;
}

void gears_move(char direction, char speed)
{
	if (direction == FORWARD){
		motorPD[0] = (speed + 15);
		motorPD[1] = (15 - speed);
	}
	else if (direction == REVERSE){
		motorPD[1] = (speed + 15);
		motorPD[0] = (15 - speed);
	}
	else if (direction == STOP){
		motorPD[1] = 15;
		motorPD[0] = 15;
	}
	else if (direction == SPIN_RIGHT){
		motorPD[1] = (speed + 15);
		motorPD[0] = (speed + 15);
	}
	else if (direction == SPIN_LEFT){
		motorPD[1] = (15 - speed);
		motorPD[0] = (15 - speed);
	}
}

void gears_init(void)
{
	int i;

	for(i = 0; i < 7; i++)
		motorPD[i] = 15;

	/* Set Mystro MOTOR to outputs */
	DDRD |= 0xfc; //Port D pins 2 - 7

	/* Set Mystro DIGITAL to inputs */
	DDRB &= 0xc0; 	//Port B pins 0 - 5	
	PORTB |= 0x3f; 	//Enable pull-ups on pins 0 - 5  

	/* Initialize Mystro ANALOG inputs */
	adc_init(ADC_AVCC, ADC_8BIT); 

	/* Set interval for 10us */
	heartbeat_init(100);

	/* Turn on universal interrupts */
	sei();
}

unsigned int analog(int grcPort)
{
	const char atmegaPort[6] = {7,6,3,2,1,0};		

	return adc_read(atmegaPort[grcPort]);

}

