/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2012.  All rights reserved

Version Date: EST 2010 Mon Oct 18 14:14:38 EDT 2010
-----------------------------------------------------------------
============================================================== */

#include <servo.h>

volatile unsigned char  masterServoCounter; 
volatile unsigned char  masterServoControl[12];

#define TOTAL_PULSES	12
#define TOTAL_PINS		6

ISR (TIMER2_COMPA_vect)
{
	// Clear last pin set
	SERVO_PORT_PRT &= ~(1 << (masterServoCounter + PORT_OFFSET));

	// Increment to next pin
	masterServoCounter++;

	// Check if counter is past last delay pulse
	if (masterServoCounter > TOTAL_PULSES)
		masterServoCounter = 0;

	// Check if counter is past last servo pulse
	if (masterServoCounter < TOTAL_PINS)
		SERVO_PORT_PRT |= (1 << (masterServoCounter + PORT_OFFSET));

	// Create delay
	OCR2A = TCNT2 + masterServoControl[masterServoCounter];
}

void servo_init(void)
{
	servo_control(ENABLE);
}

void servo_control(unsigned char value)
{
	unsigned char i;

	/* Initialize all servo control values to neutral */
	for (i = 0; i < 12; i++)
		masterServoControl[i] = SERVO_NEUTRAL;
	
	/* Initialize timer 2 with 256 prescale equals:
		20mHz and 12.8us ticks
		16mHz and 16.0us ticks
		8mHz and 32.0us ticks
	*/
	if (value == ENABLE) {
		set_bit(TIMSK2, OCIE2A); 		// Enable interrupt for matchA
		TCCR2B &= 0xF8;
		TCCR2B |= SERVO_PRESCALE;
		TCCR2A = 0;
		SERVO_PORT_DIR |= 0x3F;			// Set pins 0 - 5 as outputs
		sei();
	} 
	else if (value == DISABLE) {
		clear_bit(TIMSK2, OCIE2A);
		TCCR2B &= 0xF8;
		TCCR2B |= 0;
		SERVO_PORT_DIR &= 0xC0;			// Clear pins 0 - 5 as inputs
	}
}

/* Start with the full off pulse and add a range converted to 0 to 1000 */
void servo_position(unsigned char motor, unsigned int position)
{
	masterServoControl[motor] = 
		SERVO_FULL_OFF  + 
		(position * (SERVO_FULL_ON - SERVO_FULL_OFF)) / 1000;
}

/* Drive servos from -100 to +100 */
void servo_motor(unsigned char motor, signed char speed)
{
	speed *= -1; // This is a switch for setting CW vs. CCW
	if (speed > 100) speed = 100;
	if (speed < -100) speed = -100;
	masterServoControl[motor] = SERVO_NEUTRAL + (speed * MOTOR_FRACTION)/100;
}

void servo_robot(char direction, signed char speed)
{
	if (direction == REVERSE) {
		servo_motor(0, speed);
		servo_motor(1, (speed * -1));
	}
	else if (direction == FORWARD) {
		servo_motor(0, (speed * -1));
		servo_motor(1, speed );
	}
	else if (direction == SPIN_RIGHT) {
		servo_motor(0, speed);
		servo_motor(1, speed);
	}
	else if (direction == SPIN_LEFT) {
		servo_motor(0, (speed * -1));
		servo_motor(1, speed  * -1);
	}
	else if (direction == STOP) {
		servo_motor(0, 0);
		servo_motor(1, 0);
	}
}


