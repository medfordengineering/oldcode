/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2010.  All rights reserved

Version Date: EST 2010 Mon Oct 18 14:14:38 EDT 2010
-----------------------------------------------------------------
============================================================== */

#include <servo8.h>

unsigned char servoPortMask = 0xFF;
unsigned char  masterServoCounter; 
unsigned char  masterServoControl[9];

ISR (TIMER0_COMPA_vect)
{
	/* Use the servo master counter to designate the pin that 
	 * will be set after each pass through the interrupt. Add two 
	 * since we only support PORTD 2 - 7. */
	unsigned char servoCountMask = (1 << (masterServoCounter));

	/* Clear all pins that are designated as servo pins 
	 * in the port mask.  */
	PORTD &= servoPortMask;

	/* Check if the pin designated by the counter is 
	 * also designated as a servo pin. If it is, than 
	 * set that pin. */
	if ((servoCountMask & servoPortMask) == FALSE)
		PORTD |= servoCountMask;

	/* Update timer value based on servo value range 1000 - 2000 */
	OCR0A = TCNT0 + masterServoControl[masterServoCounter++];

	/* Note the seventh value is masterServoControl is used to create
	 * a 20ms delay between all pulses and does not set or clear a pin. */
	if (masterServoCounter == 9) masterServoCounter = 0;
}

void servo_port(unsigned char port, unsigned char state)
{
	if (state == ENABLE) {
		clear_bit(servoPortMask, port);	// Enable pin as servo
		set_bit(DDRD, port);			// Set pin as output
	} 
	else if (state == DISABLE) {
		set_bit(servoPortMask, port);	// Disable pin as servo
		clear_bit(DDRD, port);			// Set pin as input
	}
}

void servo_control(unsigned char value)
{
	unsigned char i;

	/* Initialize all servo control values to neutral */
	for (i = 0; i < 8; i++)
		masterServoControl[i] = SERVO_NEUTRAL;
	
	/* The last value in the array is used to create a 20ms 
	 * delay between all pulses. */
	masterServoControl[8] = SERVO_PULSE_DELAY;

	/* Initialize timer 0 with 64 prescale at 8mHz results
	   in 8us ticks. */
	if (value == ENABLE) {
		set_bit(TIMSK0, TMR0_MATCHA); // NEEDS COMMENTS
		TCCR0B &= 0xF8;
		TCCR0B |= TMR0_PRE64;
	} 
	else if (value == DISABLE) {
		clear_bit(TIMSK0, TMR0_MATCHA);
		TCCR0B &= 0xF8;
		TCCR0B |= CLOCK_STOP;
	}
	/* This should not be placed here */
	sei();
}

void servo_position(unsigned char motor, unsigned char position)
{
	/* Convert range of 1000 to 2000 to a scale of 0 to 180 */
	masterServoControl[motor] = 
		SERVO_FULL_OFF  + 
		((position * SERVO_MULTIPLYER) / SERVO_DEGREE_DIVISOR);
}

void servo_motor(unsigned char motor, signed char speed)
{
	if (speed > 100) speed = 100;
	/* Servo neutral + speed * multiplier / divisor */
	if (motor == MOTORA) 
		masterServoControl[2] = 187 + (speed * 62)/100;
	else if (motor == MOTORB) 
		masterServoControl[3] = 187 + (speed * 62)/100;
}

void servo_robot(char direction, signed char speed)
{
	if (direction == FORWARD) {
		servo_motor(MOTORA, speed);
		servo_motor(MOTORB, (speed * -1));
	}
	else if (direction == REVERSE) {
		servo_motor(MOTORA, (speed * -1));
		servo_motor(MOTORB, speed );
	}
	else if (direction == SPIN_LEFT) {
		servo_motor(MOTORA, speed);
		servo_motor(MOTORB, speed);
	}
	else if (direction == SPIN_RIGHT) {
		servo_motor(MOTORA, (speed * -1));
		servo_motor(MOTORB, speed  * -1);
	}
	else if (direction == STOP) {
		servo_motor(MOTORA, 0);
		servo_motor(MOTORB, 0);
	}
}

void servo_init(void)
{
	servo_control(ENABLE);
	servo_port(2, ENABLE);
	servo_port(3, ENABLE);
	sei();
}
