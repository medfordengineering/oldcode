#include <machinescience.h>
#include <adc_init.c>
#include <timer0.c>
#include <timer1.c>
#include <twi_slave.c>
#include <delay.c>
#include <hcmd_common.h>
#include <twi_addresses.h>

//FOR TESTING
#include "usart_init.c"
#include "usart_rx.c"
#include "usart_tx.c"
#include "usart_write.c"
#include "usart_string.c"
#include "ms_print.c"
#include "usart_decimal.c"
#include "usart_digits.c"

#define EN1PORT		PORTB
#define EN1DDR		DDRB
#define EN1BIT		3
#define PWM1PORT	PORTD
#define PWM1DDR		DDRD
#define PWM1BIT		6
#define INA1PORT	PORTC
#define INA1DDR		DDRC
#define INA1BIT		0
#define INB1PORT	PORTC
#define INB1DDR		DDRC
#define INB1BIT		1
#define SOL1PORT	PORTD
#define SOL1DDR		DDRD
#define SOL1BIT		3

#define EN2PORT		PORTB
#define EN2DDR		DDRB
#define EN2BIT		4
#define PWM2PORT	PORTD
#define PWM2DDR		DDRD
#define PWM2BIT		5
#define INA2PORT	PORTC
#define INA2DDR		DDRC
#define INA2BIT		2
#define INB2PORT	PORTC
#define INB2DDR		DDRC
#define INB2BIT		3
#define SOL2PORT	PORTD
#define SOL2DDR		DDRD
#define SOL2BIT		4

#define MAX_RC_WIDTH				5000			// 2000 ms
#define MIN_RC_WIDTH				2500			// 1000 ms
#define RC_SAFE_BAND				400				// 160 ms safty zone
#define MAX_RC_NEUTRAL				MID_RC_NEUTRAL + RC_SAFE_BAND	
#define MIN_RC_NEUTRAL				MID_RC_NEUTRAL - RC_SAFE_BAND
#define MID_RC_NEUTRAL				3750			// 1500 ms
#define COUNT_RC_NEUTRAL			100				// # of neutral pulses

/* Ramping */
#define FORWARD_B					0x04
#define REVERSE_B					0x08
#define FORWARD_A					0x01
#define REVERSE_A					0x02
#define MASK_MOTOR_B				0xF3
#define MASK_MOTOR_A				0xFC
#define SMOOTH_TARGET(a, b)			(a / b) * b

//#define LIMITSWITCH					3

/* NOTE:
 * channel		motor		board
 * -------------------------------
 * rc3			motorA		1
 * rc2			motorB		2
 * rc1			PnuB		3
 * rc0			PnuA		4
 */

static volatile unsigned int rcPulseWidth[4];
static volatile unsigned int rcChannel;
static volatile unsigned int rcNeutralSafeCount;
static volatile unsigned char rcNeutralSafe;
static volatile unsigned char rcControl;
static volatile unsigned char lostPulseCount;

/* Ramping */
static volatile unsigned char speedTargetA;
static volatile unsigned char speedCurrentA;
static volatile unsigned char directionTargetA;
static volatile unsigned char directionCurrentA;
static volatile unsigned char speedTargetB;
static volatile unsigned char speedCurrentB;
static volatile unsigned char directionTargetB;
static volatile unsigned char directionCurrentB;
static volatile unsigned char rampStep;
static unsigned char hcmdRamping;

/* BUG: Need to update the following function names with their
 * cooresponding functions. */

static inline void hcmd_stop_robot(unsigned char);
static inline void set_rcChannel(unsigned char);
static void hcmd_drive_pneumatic(unsigned char, unsigned char );
static void hcmd_initialize_pins(void);
static void hcmd_initialize_limitswitch(char);
static void hcmd_initialize_rc(char);
static inline void hcmmod_noramp(unsigned char, signed int );
static inline void hcmmod_ramp(unsigned char , signed int );
static void reset_pulse_buffer(void);
static void hcmd_robot_noramp(unsigned char , unsigned int );

/* Simple limit switch for the four channel input. If any
 * channel is set low than the robot is automatically stoped.
 * BUG: This should be much more sophisticated. 
 */

ISR (PCINT0_vect)
{
	if (test_bit(PINB, 0) == 0){
			PORTC &= MASK_MOTOR_A;		
			clear_bit(PORTB, 3);		// Clear bit sends a kill to motor a

			PORTC &= MASK_MOTOR_B;		
			clear_bit(PORTB, 4);		// Clear bit sends a kill to motor b
	}
}

/* Increments every 2ms and is cleared on each interrupt by
 * input capture. If count reaches 40ms without a clear than the
 * RC signal has been lost. */

ISR (TIMER1_COMPA_vect)
{
	if (lostPulseCount++ > 20){
		lostPulseCount = 0;					// Reset pulse counter
		reset_pulse_buffer();			// Set all pulses to neutral
		set_bit(PORTD, 2);				// Turn off RC safe LED
		rcNeutralSafe = FALSE;			// Set neutral flag to false
		rcNeutralSafeCount = 0;		// Set
		hcmd_robot_noramp(FORWARD, 0);	// SHOULD BE FULL KILL
	}
	OCR1A += 5000;					// 4ns count * 5000 = 2ms
}

ISR (TIMER1_CAPT_vect)
{
	static unsigned int pulseCount;
	unsigned int pulseWidth;
	signed int rcSpeed;

	cli();	

	/* Captured on high transistion and set next capture for low */
	if (test_bit(TCCR1B, ICES1)) {	
		pulseCount = ICR1;			
		clear_bit(TCCR1B, ICES1);
	} 
	/* Captured on low transition */
	else {							

		/* Store length of pulse */
		pulseWidth = ICR1 - pulseCount;

		//	usart_digits(pulseWidth, BASE_10, UNSIGNED_ZEROS, 4);
		//	usart_write(CARRIAGE_RETURN);
		//	usart_write(LINE_FEED);

		/* Check if pulse is in standard servo range */
		if ((pulseWidth < MAX_RC_WIDTH) && (pulseWidth > MIN_RC_WIDTH)) {

			/* Reset lost pulse counter to zero */
			lostPulseCount = 0;

			// Check if all four servo pulses are neutral for a defined count. //
			if 	(	((rcPulseWidth[0] < MAX_RC_NEUTRAL) && (rcPulseWidth[0] > MIN_RC_NEUTRAL)) &&
			 		((rcPulseWidth[1] < MAX_RC_NEUTRAL) && (rcPulseWidth[1] > MIN_RC_NEUTRAL)) &&
			 		((rcPulseWidth[2] < MAX_RC_NEUTRAL) && (rcPulseWidth[2] > MIN_RC_NEUTRAL)) &&
			 		((rcPulseWidth[3] < MAX_RC_NEUTRAL) && (rcPulseWidth[3] > MIN_RC_NEUTRAL)) 	) {

				if (rcNeutralSafeCount++ > COUNT_RC_NEUTRAL) {
					clear_bit(PORTD, 2);
					rcNeutralSafe = TRUE;
				}
			}

			if ((rcNeutralSafe == TRUE) && (rcControl == TRUE)) {
	
				// Convert pulse width (2500 to 5000) to speed values (0 - 255)
				rcSpeed = ((signed int) pulseWidth - MID_RC_NEUTRAL) / 2;
				if (rcSpeed > +255) rcSpeed = +255;
				if (rcSpeed < -255) rcSpeed = -255;
				
				// THIS LINE IS USED TO REMOVE SPURIOUS SIGNALS CAN BE ADJUSTED 
				// if ((pulseWidth - rcPulseWidth[rcChannel]) < 200) {

				if (rcChannel == 2) {
					if (hcmdRamping == FALSE) 
						hcmmod_noramp(MOTORB, rcSpeed);
					else 
						hcmmod_ramp(MOTORB, rcSpeed);
				}
				else if (rcChannel == 3) {
					if (hcmdRamping == FALSE) 
						hcmmod_noramp(MOTORA, rcSpeed);
					else 
						hcmmod_ramp(MOTORA, rcSpeed);
				}
				else if (rcChannel == 0) {
					if (rcSpeed < -10) 
						set_bit(PORTD, 3);		// On	
					else
						clear_bit(PORTD, 3); 	// Off
				}
				else if (rcChannel == 1) {
					if (rcSpeed < -10) 
						set_bit(PORTD, 4);		// On
					else
						clear_bit(PORTD, 4);	// Off
				}
//				}
			}

			/* Store pulse in channel buffer */
			rcPulseWidth[rcChannel++] = pulseWidth;

			/* Change channel */
			if (rcChannel >= 4) rcChannel = 0;
			set_rcChannel(rcChannel);
		}
		else {
			// TESTING
			//usart_digits(pulseWidth, BASE_10, UNSIGNED_ZEROS, 4);
			//usart_write(CARRIAGE_RETURN);
			//usart_write(LINE_FEED);
		}
		/* Set next capture for high */
		set_bit(TCCR1B, ICES1);		
	}
	sei();
}

/* BUG: Ramping not working yet. */
ISR (TIMER2_COMPA_vect)
{
	/* If speed and direction match do nothing. */
	if ((OCR0A != speedTargetA) || (directionCurrentA != directionTargetA)) {
		/* Enable output on drive A. */
		set_bit(EN1PORT, EN1BIT);
		/* If direction does not match than reduce speed EXCEPT if speed is 0. */
		if (directionCurrentA != directionTargetA) {
			if (OCR0A) 
				OCR0A -= rampStep;
		}
		/* If direction matches increase or decrease speed depending on target speed. */
		else {
			if (speedTargetA > OCR0A) 
				OCR0A += rampStep;
			else 
				OCR0A -= rampStep;
		}
		/* If speed = 0 than reverse direction. */
		if (OCR0A == 0) {
			PORTC &= MASK_MOTOR_A;
			if (directionCurrentA == REVERSE) {
				directionCurrentA = FORWARD;
				PORTC |= FORWARD_A;
			}
			else if (directionCurrentA == FORWARD) {
				directionCurrentA = REVERSE;
				PORTC |= REVERSE_A;
			}
		}
		//usart_write(OCR0A);
		//usart_write(directionCurrentA);
		//usart_write(PORTC);
	}
	if ((OCR0B != speedTargetB) || (directionCurrentB != directionTargetB)) {
		set_bit(EN2PORT, EN2BIT);
		if (directionCurrentB != directionTargetB) {
			if (OCR0B) 
				OCR0B -= rampStep;
		}
		else {
			if (speedTargetB > OCR0B) 
				OCR0B += rampStep;
			else 
				OCR0B -= rampStep;
		}
		if (OCR0B == 0) {
			PORTC &= MASK_MOTOR_B;
			if (directionCurrentB == REVERSE) {
				directionCurrentB = FORWARD;
				PORTC |= FORWARD_B;
			}
			else if (directionCurrentB == FORWARD) {
				directionCurrentB = REVERSE;
				PORTC |= REVERSE_B;
			}
		}
		//usart_write(OCR0B);
		//usart_write(directionCurrentB);
		//
		
		//usart_write(PORTC);
	}
	OCR2A = TCNT2 + 10;
}

void hcmd_initialize_pins(void)
{
/* Disable motor drivers on set up. */
	clear_bit(EN1PORT, EN1BIT);	
	clear_bit(EN2PORT, EN2BIT);	
	clear_bit(SOL1PORT, SOL1BIT);	
	clear_bit(SOL2PORT, SOL2BIT);	

/* Set direction, PWM, enable and solenoid pins as outputs for both motors. */
	set_bit(EN1DDR, EN1BIT);
	set_bit(INA1DDR, INA1BIT);
	set_bit(INB1DDR, INB1BIT);
	set_bit(PWM1DDR, PWM1BIT);
	set_bit(SOL1DDR, SOL1BIT);

	set_bit(EN2DDR, EN2BIT); 
	set_bit(INA2DDR, INA2BIT); 
	set_bit(INB2DDR, INB2BIT);
	set_bit(PWM2DDR, PWM2BIT); 
	set_bit(SOL2DDR, SOL2BIT); 

/* Set multiplexor control pins */
	clear_bit(DDRB, 0);		// Set mux-input pin as input
	set_bit(DDRB, 1);		// Channel select pin
	set_bit(DDRB, 2);		// Channel select pin
	set_bit(DDRD, 2);		// Servo safe indicator LED
}

void hcmd_initialize_limitswitch(char state)
{
	if (state == ENABLE) {
		set_bit(PORTB, 0);			// Set pullup for mux-input pin
		set_bit(PCMSK0, PCINT0);  	// Set mux-input for interrupt
		set_bit(PCICR, PCIE0);		// "						"
	}
	else if (state == DISABLE) {
		clear_bit(PORTB, 0);		// Set pullup for mux-input pin
		clear_bit(PCMSK0, PCINT0); 	// Set mux-input for interrupt
		clear_bit(PCICR, PCIE0);	// "						"
	}
}

static void reset_pulse_buffer(void)
{
	int i;
	for (i = 0; i < 4; i++)
		rcPulseWidth[i] = MID_RC_NEUTRAL;	// Set all pulses to neutral

	rcChannel =  0;							// Reset RC channel to zero
	set_rcChannel(rcChannel);				

	lostPulseCount = 0;						// Reset pulse counter to zero
	rcNeutralSafe = FALSE;					// Set neutral flag to false
	rcNeutralSafeCount = 0;				// Reset neutral counter to zero
	set_bit(PORTD, 2);						// Turn off servo safe indicator LED

	//hcmd_robot_noramp(FORWARD, 0);	// SHOULD BE FULL KILL
}

void hcmd_initialize_rc(char state)
{
	if (state == ENABLE) {
		TCCR1B = 	(1<<ICNC1)|			// Add noise canceller for input capture	
					(1<<ICES1)|			// Set for capture on rising edge
					(1<<CS11);			// Div 8 prescale

		TIMSK1 = 	(1<<ICIE1)|			// Set interrupt for input capture unit
					(1<<OCIE1A);		// Set interrupt for lost pulse counter

		// FORMAT SAME ABOVE
		//set_bit(TIMSK1, ICIE1); 		// Set interrupt for input capture unit 
		//set_bit(TIMSK1, OCIE1A);			// Set interrupt for match on comp A, lost pulse
	
		//set_bit(PORTD, 2);				// Turn off servo safe indicator LED

		// MAKE THIS PART OF A WHOLE RESET ROUTINE
		//rcChannel =  0;
		//set_rcChannel(rcChannel);
		//reset_pulse_buffer();
		/*
		for (i = 0; i < 4; i++)
		//	rcPulseWidth[i] = 3750;
			rcPulseWidth[i] = MID_RC_NEUTRAL;
		*/

	}
	else if (state == DISABLE) {
		TCCR1B &= 0xF8;					// Clear prescale bits turns off clock
		TCCR1B |= CLOCK_STOP;
		clear_bit(TIMSK1, ICIE1);		// Disable interrupt for input capture	
	}
}

//INIALIZE PWM
void hcmd_initialize_timers(void)
{
//	timer0_pwm_init(TMR_MODE3, TMR0_PRE8);

	/* Set timer0 pins to output. */
	set_bit(DDRD, 6);
	set_bit(DDRD, 5);

	/* Clear on Match for A and B. Fast PWM or mode 3. Div8 prescale. */
	TCCR0A = 	(1<<COM0A1) | (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);	
	TCCR0B = 	(1<<CS01);

	/* Initialize speed to 0. */
	OCR0A = 0;
	OCR0B = 0;
}

void hcmd_initialize_adc(void)
{
	// LOCALIZE THIS
	adc_init(ADC_AVCC, ADC_10BIT);
}

void hcmd_initialize_ramping(unsigned char state)
{
	if (state == ENABLE) {
		TCCR2B &= 0xF8; 	
		//TCCR2B |= TMR2_PRE1024;		// Prescale 1024
		TCCR2B |= TMR2_PRE64;
		set_bit(TIMSK2, OCIE2A);	// Enable interrupt on Match A
		hcmdRamping = TRUE;
	}
	else if (state == DISABLE) {
		TCCR2B &= 0xF8; 	
		TCCR2B |= CLOCK_STOP;		// Stop timer 2 clock
		clear_bit(TIMSK2, OCIE2A);	// Disable interrupt on Match A
		hcmdRamping = FALSE;
	}
}

static inline void set_rcChannel(unsigned char channel)
{
	PORTB &= 0xF9;
	PORTB |= (rcChannel << 1);
}

static inline void hcmmod_noramp(unsigned char side, signed int speed)
{	
	if (side == MOTORA) {
		set_bit(EN1PORT, EN1BIT);
		PORTC &= MASK_MOTOR_A;
		if (speed > 0) 
			 PORTC |= FORWARD_A;
		else {
			 PORTC |= REVERSE_A;
			 speed *= -1;
		}
		OCR0A = speed;
	}
	else if (side == MOTORB) {
		set_bit(EN2PORT, EN2BIT);
		PORTC &= MASK_MOTOR_B;
		if (speed > 0) 
			 PORTC |= FORWARD_B;
		else {
			 PORTC |= REVERSE_B;
			 speed *= -1;
		}
		OCR0B = speed;
	}
}

static inline void hcmmod_ramp(unsigned char side, signed int speed)
{
	if (side == MOTORA) {
		set_bit(EN1PORT, EN1BIT);
		if (speed > 0) {
			speedTargetA = speed;
			directionTargetA = FORWARD;
		}
		else {
			speedTargetA = (speed * -1);
			directionTargetA = REVERSE;
		}
	}
	else if (side == MOTORB) {
		set_bit(EN2PORT, EN2BIT);
		if (speed > 0) {
			speedTargetB = speed;
			directionTargetB = FORWARD;
		}
		else {
			speedTargetB = (speed * -1);
			directionTargetB = REVERSE;
		}
	}
}

void hcmd_robot_noramp(unsigned char direction, unsigned int speed)
{
	PORTC &= 0xF0;
	if (direction == FORWARD) PORTC |= 0x05;
	else if (direction == REVERSE) PORTC |= 0x0A;
	else if (direction == SPIN_RIGHT) PORTC |= 0x06;
	else if (direction == SPIN_LEFT) PORTC |= 0x09;

	OCR0A = speed;
	OCR0B = speed;

	set_bit(EN1PORT, EN1BIT);
	set_bit(EN2PORT, EN2BIT);  		
}

//NOTE YOU NEED TO SET SPEED TO ZERO TO TURN OFF LIGHTS
//IMPORTANT: THIS FUNCTION MAY NO LONGER WORK?
static inline void hcmd_stop_robot(unsigned char state)
{
	PORTC &= 0xF0;
	if (state == BREAK) {
		set_bit(EN1PORT, EN1BIT);
		set_bit(EN2PORT, EN2BIT);  		
	}
	else if (state == KILL) {
		clear_bit(EN1PORT, EN1BIT);
		clear_bit(EN2PORT, EN2BIT);  		
		OCR0A = 0;
		OCR0B = 0;
		//timer0_duty(0, 'A');
		//timer0_duty(0, 'B');
	}
}

static inline void hcmd_stop_motor(unsigned char side, unsigned char state)
{
	// SETTING ENABLE PIN SHOULD BE A MACRO
/* Clears both direction pins or MotorA or MotorB. */
/* Sets enable pin on BREAK and clears enable pin on KILL. */
	if (side == MOTORA) {
		//PORTC &= 0xFC;
		PORTC &= MASK_MOTOR_A;
		if (state == BREAK) set_bit(PORTB, 3);
		if (state == KILL) clear_bit(PORTB, 3);
	}
	else if (side == MOTORB) {
		//PORTC &= 0xF3;
		PORTC &= MASK_MOTOR_B;
		if (state == BREAK) set_bit(PORTB, 4);
		if (state == KILL) clear_bit(PORTB, 4);
	}
}

void hcmd_drive_pneumatic(unsigned char side, unsigned char state)
{
	if (side == SOLENOIDA) {
		if (state == OFF) clear_bit(PORTD, 3);
		else if (state == ON) set_bit(PORTD, 3);
	}
	else if (side == SOLENOIDB) {
		if (state == OFF) clear_bit(PORTD, 4);
		else if (state == ON) set_bit(PORTD, 4);
	}
}

unsigned int hcmd_read_current(unsigned char side)
{
	//unsigned int voltage, current;

	if (side == MOTORA)
		return (adc_read(6) * 100) / 6.4;
		//voltage = adc_read(6);
	else if (side == MOTORB)
		return (adc_read(7) * 100) / 6.4;
	else
		return -1;
		//voltage = adc_read(7);
	//current = (voltage * 100) / 6.4;
	//return current;
}

/* Convert speed from +-100 to +-255. Limit values to +-100.
 * BUG: This might break with values over +-128. */

static signed int speed_conversion(signed int speed)
{
	if (speed > 100) speed = 100;
	if (speed < -100) speed = -100;
	return (speed * 0xFF) / 100;
}

int main(void)
{
	unsigned int current;
	unsigned int rcPulse = 0;
	signed int rcSpeed;
	signed int tSpeed;

	/* DEBUGGING: */
	usart_init(38400);

	directionCurrentA = FORWARD;
	directionCurrentB = FORWARD;
	directionTargetA = FORWARD;
	directionTargetB = FORWARD;

	speedTargetA = 0;
	speedTargetB = 0;

	hcmdRamping = TRUE;
	rampStep = 1;

	rcNeutralSafeCount = 0;
	rcNeutralSafe = FALSE;
	rcControl = TRUE;

	hcmd_initialize_pins();
	hcmd_initialize_timers();
	hcmd_initialize_adc();
	hcmd_initialize_rc(ENABLE);
	//hcmd_initialize_ramping(ENABLE);
	hcmd_initialize_ramping(DISABLE);
	hcmdRamping = FALSE;

	//set_bit(PORTB, 0);			// Set pullup for mux-input pin

	/* Sets direction and a speed of 1. We must prime the pump with a 1 so that
	 * there is an initial direction * that is not zero. */
	hcmmod_noramp(MOTORA, 1); 
	hcmmod_noramp(MOTORB, 1);

	twiSlaveInit(MOTOR_MODULE);
	sei();

	while (TRUE) {
		if (twiDataInBuffer) {
			
			switch (twiData[0]) {
				case CMD_MOTOR_DRIVEMOTOR:
//					usart_digits((signed char) twiData[2], BASE_10, SIGNED_ZEROS, 4);
//					usart_write(':');
					tSpeed = speed_conversion((signed char) twiData[2]);
//					usart_digits(tSpeed, BASE_10, SIGNED_ZEROS, 4);
//					usart_write(CARRIAGE_RETURN);
//					usart_write(LINE_FEED);
					if (hcmdRamping == TRUE)
						hcmmod_ramp(twiData[1], tSpeed);
					else 
						hcmmod_noramp(twiData[1], tSpeed);
				break;
				case CMD_MOTOR_STOPMOTOR:
					if (twiData[1] == MOTORA) {
						PORTC &= MASK_MOTOR_A;
						if (twiData[2] == BREAK) set_bit(PORTB, 3);
						if (twiData[2] == KILL) clear_bit(PORTB, 3);
					}
					else if (twiData[1] == MOTORB) {
						PORTC &= MASK_MOTOR_B;
						if (twiData[2] == BREAK) set_bit(PORTB, 4);
						if (twiData[2] == KILL) clear_bit(PORTB, 4);
					}
				break;
				case CMD_MOTOR_DRIVEROBOT:
					//hcmd_robot_noramp(twiData[1], (twiData[2] * 0xFF) / 100);
					hcmd_robot_noramp(twiData[1], speed_conversion((signed char) twiData[2])); 
				break;
				case CMD_MOTOR_STOPROBOT:
					hcmd_stop_robot(twiData[1]);
				break;
				case CMD_MOTOR_PNEUMATIC:
					hcmd_drive_pneumatic(twiData[1], twiData[2]);
				break;
				case CMD_MOTOR_CURRENT:
					current = hcmd_read_current(twiData[1]);
					twiData[0] = (current >> 8) & 0x00ff;
					twiData[1] = current & 0x00ff;
				break;
				case CMD_MOTOR_PULSE:
					rcPulse =  (rcPulseWidth[(unsigned int)twiData[1]]) / 2.5;
					twiData[0] = (rcPulse >> 8); 
					twiData[1] = rcPulse; 
				break;
				case CMD_MOTOR_RCMOTOR:
						/* Convert from defines for MOTORA etc to the corresponding channels. */
						if (twiData[1] == 0) twiData[1] = 3;
						//else if (twiData[1] == 1) twiData[1] = 2;
						if (twiData[1] == 1) twiData[1] = 2;
						rcSpeed =  ((signed int) rcPulseWidth[(unsigned int)twiData[1]] - MID_RC_NEUTRAL) / 9;
						twiData[0] = (rcSpeed >> 8); 
						twiData[1] = rcSpeed; 
				break;
				case CMD_MOTOR_RCPNEUMATIC:
						/* Convert from defines for PNEU etc to the corresponding channels. */
						if (twiData[1] == 2) twiData[1] = 0;
						//else if (twiData[1] == 3) twiData[1] = 1;
						if (twiData[1] == 3) twiData[1] = 1;
						if (((signed int) rcPulseWidth[(unsigned int)twiData[1]] - MID_RC_NEUTRAL) < -10)
							twiData[0] = ON;
						else
							twiData[0] = OFF;
				break;
				case CMD_HCMD_WRITE_STATE:
					if (twiData[1] == DISABLE) {
						rcControl = FALSE;
						hcmd_initialize_rc(DISABLE);
					}
					else if (twiData[1] == MONITOR) {
						rcControl = FALSE;
						hcmd_initialize_rc(ENABLE);
					}
					else {
						rcControl = TRUE;
						hcmd_initialize_rc(ENABLE);
					}
				break;
				case CMD_HCMD_READ_STATE:
					twiData[0] = rcNeutralSafe;
				break;
				case CMD_HCMD_RAMPING:
					hcmd_initialize_ramping(twiData[1]);		// Enable or disable ramping
				break;
				case CMD_MOTOR_DRIVELIMIT:
					hcmd_initialize_rc(DISABLE); 			// See BUG below	
					rcChannel = (4 - twiData[3]);
					set_rcChannel(rcChannel);
					hcmd_initialize_limitswitch(ENABLE);
					//tSpeed = ((signed char) twiData[2] * 0xFF) / 100;
					tSpeed = speed_conversion((signed char) twiData[2]);
					hcmmod_noramp(twiData[1], tSpeed);
				break;

				/* BUG: Sending channel 1, 2, or 3 results in that channel be
				 * correctly polled, but also channel 4. */

				/* BUG: Disabling the RC seems to be necessary but I am not
				 * sure why. */

				/* BUG: The function disables both motors. It should only
				 * disable the one being called. */

				case CMD_MOTOR_DRIVEBLOCK:
					hcmd_initialize_rc(DISABLE); 		
					set_bit(PORTB, 0);				//Set mux for input
					rcChannel = (4 - twiData[3]);
					set_rcChannel(rcChannel);
					if (test_bit(PINB, 0) != 0) {
					//	tSpeed = ((signed char) twiData[2] * 0xFF) / 100;
						tSpeed = speed_conversion((signed char) twiData[2]);
						hcmmod_noramp(twiData[1], tSpeed);
					}
					else {
							PORTC &= MASK_MOTOR_A;		
							clear_bit(PORTB, 3);

							PORTC &= MASK_MOTOR_B;	
							clear_bit(PORTB, 4);		
					}
				break;

			}
			TWCR = TWISLAVEACTIVEACK;
			twiDataInBuffer = FALSE;
		}
	}
}

