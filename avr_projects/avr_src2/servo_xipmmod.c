/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2010.  All rights reserved

Version Date: EST 2010 Mon Oct 18 14:14:38 EDT 2010
-----------------------------------------------------------------
============================================================== */

#include <servo_xipmmod.h>

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
	port += 2;
	if (state == ENABLE) {
		clear_bit(servoPortMask, port);	// Enable pin as servo
		set_bit(DDRD, port);			// Set pin as output
	} 
	else if (state == DISABLE) {
		set_bit(servoPortMask, port);	// Disable pin as servo
		clear_bit(DDRD, port);			// Set pin as input
	}
}

void servo_setup(unsigned char value)
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
		set_bit(TIMSK0, TMR0_MATCHA); // Enable interrupt for matchA
		TCCR0B &= 0xF8;
		TCCR0B |= SERVO_PRESCALE;
	} 
	else if (value == DISABLE) {
		clear_bit(TIMSK0, TMR0_MATCHA);
		TCCR0B &= 0xF8;
		TCCR0B |= CLOCK_STOP;
	}
	/* This should not be placed here */
	sei();
}

/* Start with the full off pulse and add a range converted to 0 to 1000 */
void servo_position(unsigned char motor, unsigned int position)
{
	motor += 2;
	masterServoControl[motor] = 
		SERVO_FULL_OFF  + 
		(position * (SERVO_FULL_ON - SERVO_FULL_OFF)) / 1000;
}

void servo_motor(unsigned char motor, signed char speed)
{
	motor += 2;
	speed *= -1; // This is a switch for setting CW vs. CCW
	if (speed > 100) speed = 100;
	masterServoControl[motor] = SERVO_NEUTRAL + (speed * MOTOR_FRACTION)/100;
}

void gears_motor(unsigned char motor, signed char speed)
{
	motor += 2;
	if (speed > 100) speed = 100;
	masterServoControl[motor] = SERVO_NEUTRAL + (speed * GEARS_FRACTION)/100;
}

void servo_robot(char direction, signed char speed)
{
	if (direction == REVERSE) {
		servo_motor(MOTORA, speed);
		servo_motor(MOTORB, (speed * -1));
	}
	else if (direction == FORWARD) {
		servo_motor(MOTORA, (speed * -1));
		servo_motor(MOTORB, speed );
	}
	else if (direction == SPIN_RIGHT) {
		servo_motor(MOTORA, speed);
		servo_motor(MOTORB, speed);
	}
	else if (direction == SPIN_LEFT) {
		servo_motor(MOTORA, (speed * -1));
		servo_motor(MOTORB, speed  * -1);
	}
	else if (direction == STOP) {
		servo_motor(MOTORA, 0);
		servo_motor(MOTORB, 0);
	}
}

void gears_robot(char direction, signed char speed)
{
	if (direction == FORWARD) {
		gears_motor(MOTORA, speed);
		gears_motor(MOTORB, (speed * -1));
	}
	else if (direction == REVERSE) {
		gears_motor(MOTORA, (speed * -1));
		gears_motor(MOTORB, speed );
	}
	else if (direction == SPIN_LEFT) {
		gears_motor(MOTORA, speed);
		gears_motor(MOTORB, speed);
	}
	else if (direction == SPIN_RIGHT) {
		gears_motor(MOTORA, (speed * -1));
		gears_motor(MOTORB, speed  * -1);
	}
	else if (direction == STOP) {
		gears_motor(MOTORA, 0);
		gears_motor(MOTORB, 0);
	}
}

void servo_init(void)
{
	servo_setup(ENABLE);
	servo_port(0, ENABLE);
	servo_port(1, ENABLE);
	sei();
}

void servo_control(unsigned char value)
{
	if (value == ENABLE)
		servo_init();
	else
		servo_setup(DISABLE);
}
