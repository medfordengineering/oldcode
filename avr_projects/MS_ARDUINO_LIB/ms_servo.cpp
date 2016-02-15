/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2012.  All rights reserved

Version Date: EST 2010 Mon Oct 18 14:14:38 EDT 2010
-----------------------------------------------------------------
============================================================== */

#include <ms_servo.h>

unsigned char servoPortMask = 0xFF;
unsigned char  masterServoCounter; 
unsigned char  masterServoControl[9];


ISR (TIMER2_COMPA_vect)
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
	//OCR2A = TCNT2 + masterServoControl[masterServoCounter++];
	OCR2A = TCNT2 + masterServoControl[masterServoCounter++];

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

	/* Initialize timer 2 with 256 prescale equals:
		20mHz and 12.8us ticks
		16mHz and 16.0us ticks
		8mHz and 32.0us ticks
	 */
	if (value == ENABLE) {
		set_bit(TIMSK2, OCIE2A); // Enable interrupt for matchA
		TCCR2B &= 0xF8;
		TCCR2B |= SERVO_PRESCALE;
	} 
	else if (value == DISABLE) {
		clear_bit(TIMSK2, OCIE2A);
		TCCR2B &= 0xF8;
		TCCR2B |= STOP;
	}
}

/* Start with the full off pulse and add a range converted to 0 to 1000 */
void servo_position(unsigned char motor, unsigned int position)
{
	motor += 2;
	masterServoControl[motor] = 
		SERVO_FULL_OFF  + 
		(position * (SERVO_FULL_ON - SERVO_FULL_OFF)) / 1000;
}

/* Drive servos from -100 to +100 */
void servo_motor(unsigned char motor, signed char speed)
{
	motor += 2;
	speed *= -1; // This is a switch for setting CW vs. CCW
	if (speed > 100) speed = 100;
	if (speed < -100) speed = -100;
	masterServoControl[motor] = SERVO_NEUTRAL + (speed * MOTOR_FRACTION)/100;
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

/* Sets up servo control on default ports D2 and D3 */
void servo_init(void)
{
	servo_setup(ENABLE);
	servo_port(0, ENABLE);
	servo_port(1, ENABLE);
	TCCR2A = 0;
	sei();
}

/* For backwards compatability */ 
void servo_control(unsigned char value)
{
	if (value == ENABLE)
		servo_init();
	else
		servo_setup(DISABLE);
}
