/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2010.  All rights reserved

Version Date: Fri Feb 12 16:48:12 EST 2010
-----------------------------------------------------------------
============================================================== */

#include <mastermod.h>

uint8_t servoPortMask = 0xFF;
int16_t masterServoCounter; 
int16_t masterServoControl[7];

ISR (TIMER1_COMPA_vect)
{
	/* Use the servo master counter to designate the pin that 
	 * will be set after each pass through the interrupt. Add two 
	 * since we only support PORTD 2 - 7. */
	uint8_t servoCountMask = (1 << (masterServoCounter + 2));

	/* Clear all pins that are designated as servo pins 
	 * in the port mask.  */
	PORTD &= servoPortMask;

	/* Check if the pin designated by the counter is 
	 * also designated as a servo pin. If it is, than 
	 * set that pin. */
	if ((servoCountMask & servoPortMask) == FALSE)
		PORTD |= servoCountMask;

	/* Update timer value based on servo value range 2500 - 5000 */
	OCR1A = TCNT1 + masterServoControl[masterServoCounter++];

	/* Note the seventh value is masterServoControl is used to create
	 * a 20ms delay between all pulses and does not set or clear a pin. */
	if (masterServoCounter == 7) masterServoCounter = 0;
}

void servo_control(char state)
{
	if (state == ENABLE) {
		servo_init(ON);
		servo_port(0, SERVO);
		servo_port(1, SERVO);
		servo_port(2, SERVO);
		servo_port(3, SERVO);
		servo_port(4, SERVO);
		servo_port(5, SERVO);
		sei();
	}
	else if (state == DISABLE) {
		servo_init(OFF);
		servo_port(0, INPUT);
		servo_port(1, INPUT);
		servo_port(2, INPUT);
		servo_port(3, INPUT);
		servo_port(4, INPUT);
		servo_port(5, INPUT);
		cli();
	}
}

void servo_port(int8_t port, int8_t function)
{
	/* Shift 2 since only PORTS 2 - 7 are used */
	port += 2;
	if (function == SERVO) {
		clear_bit(servoPortMask, port);	// Enable pin as servo
		set_bit(DDRD, port);			// Set pin as output
	} 
	else if (function == INPUT) {
		set_bit(servoPortMask, port);	// Disable pin as servo
		clear_bit(DDRD, port);			// Set pin as input
		set_bit(PORTD, port);			// Enable pull ups
	}
	else if (function == OUTPUT) {
		set_bit(servoPortMask, port);	// Disable pin as servo
		set_bit(DDRD, port);			// Set pin as output
	}
}

void servo_init(int8_t value)
{
	int8_t i;

	/* Initialize all servo control values to neutral */
		for (i = 0; i < 6; i++)
			masterServoControl[i] = SERVO_NEUTRAL;
	
	/* The last value in the array is used to create a 20ms 
	 * delay between all pulses. */
		masterServoControl[6] = SERVO_PULSE_DELAY;

	/* Initialize timer 1 with 8 prescale at 20mHz results
	   in 400ns ticks. So, 2500 ticks equals 1000us and
	   5000 ticks equals 2000us. */
	if (value == ON) {
		set_bit(TIMSK1, TMR1_MATCHA); // NEEDS COMMENTS
		TCCR1B &= 0xF8;
		TCCR1B |= TMR1_PRE8;
	} 
	else if (value == OFF) {
		clear_bit(TIMSK1, TMR1_MATCHA);
		TCCR1B &= 0xF8;
		TCCR1B |= CLOCK_STOP;
	}
}

void servo_position(int8_t motor, int16_t placement)
{
	/* Convert range of 2500 to 5000 to a scale of 0 to 1000 */
	masterServoControl[motor] = SERVO_FULL_OFF  + ((placement * SERVO_MULTIPLYER) / SERVO_DIVISOR);
}

void servo_motor(unsigned char motor, signed int speed)
{
	/* In the forward direction the range is 3750 (no spin)
	   to 5000 for full spin. This needs to be converted to
	   from a range of 0 to 100. For reverse the range is 2500
	   to 3750.
	*/
	speed = (speed * 20) / 75;
	if (speed > 100) speed = 100;
	masterServoControl[motor] = SERVO_NEUTRAL + (speed * -12.5);
}

void servo_stop(int8_t motor)
{
	masterServoControl[motor] = SERVO_NEUTRAL;
}

void digital_port(int8_t port, int8_t direction)
{
	if (direction == OUTPUT) 
		set_bit(DDRB, port);
	else if (direction == INPUT) {
		clear_bit(DDRB, port);
		set_bit(PORTB, port); 		//Set pull-ups on input pins
	}
}

void analog_input(unsigned char state)
{
	/* Set ADC for ADVCC or supply voltage reference and set for
	 * 10 bit resolution */
	ADMUX = (1<<REFS0) | (0<<REFS1) | (0<<ADLAR);

	/* Enable ADC and set clock to 62.5kHz */
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);	
}

unsigned int adc_convert(char channel)
{
	/* Select ADC channel */
	ADMUX &= 0xF0;	
	ADMUX |= channel;

	/* Start ADC conversion */
	set_bit(ADCSRA, ADSC);

	/* Wait for conversion to complete */
	while (test_bit(ADCSRA, ADSC));

	/* Return low and high ADC value combined */
	return ADCW;
}

unsigned int analog_read(int port)
{
	unsigned long analogValue = 0;
	unsigned int i;
	const char masterPortConvert[6] = {7,6,3,2,1,0};		

	/* Sum the reading samples. */
	for (i = 0; i < 1000; i++)
		analogValue += adc_convert(masterPortConvert[port]);
	
	/* Return averaged value. */
	return analogValue /= 1000;
}

