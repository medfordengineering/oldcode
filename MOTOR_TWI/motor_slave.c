#include <machinescience.h>
#include <twi_slave.c>

#define FORWARD 				0x00
#define REVERSE					0x01
#define SPIN_RIGHT				0x02
#define SPIN_LEFT 				0x03
#define MOTORA					0x04
#define MOTORB					0x01
#define MOTORC					0x02
#define MOTORD					0x03
#define SOLENOIDA				0x02
#define SOLENOIDB				0x03
#define SOLENOIDC				0x04
#define SOLENOIDD				0x05
#define COAST					0x00
#define BRAKE					0x01
#define STOP					0x04		

#define CMD_MOTOR_DRIVEMOTOR	0x37
#define CMD_MOTOR_PNEUMATIC		0x38
#define CMD_MOTOR_CURRENT		0x39
#define CMD_MOTOR_ISRCSIG		0x3A
#define CMD_MOTOR_READRCSIG		0x3B
#define CMD_MOTOR_DRIVEROBOT	0x48
#define CMD_MOTOR_STOPROBOT		0x49
#define CMD_MOTOR_STOPMOTOR		0x4A
#define CMD_MOTOR_PULSE			0x4B
#define CMD_MOTOR_RCMOTOR		0x4C
#define CMD_MOTOR_RCPNEUMATIC 	0x4D
#define CMD_HCMD_READ_STATE		0x4E
#define CMD_HCMD_WRITE_STATE	0x4F
#define CMD_MOTOR_DRIVELIMIT	0x50
#define CMD_HCMD_RAMPING		0x51
#define CMD_MOTOR_DRIVEBLOCK	0x52
#define CMD_MOTOR_DRIVERAW		0x53	

#define MONITOR					0x02

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
#define SOLENOID_OFF_BAND			-10	

#define FORWARD_B					0x04
#define REVERSE_B					0x08
#define FORWARD_A					0x01
#define REVERSE_A					0x02
#define MASK_MOTOR_B				0xF3
#define MASK_MOTOR_A				0xFC

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
static volatile unsigned int neutralPulseCount;
static volatile unsigned char rcSafe;
static volatile unsigned char rcControl;
static volatile unsigned char lostPulseCount;

/* BUG: Need to update the following function names with their
 * cooresponding functions. */
static inline void hcmd_stop_robot(unsigned char);
static inline void set_rcChannel(unsigned char);
static void hcmd_drive_pneumatic(unsigned char, unsigned char );
static void hcmd_initialize_pins(void);
static void hcmd_initialize_limitswitch(char);
static void hcmd_initialize_rc(char);
static inline void hcmmod_noramp(unsigned char, signed int );
//static inline void hcmmod_ramp(unsigned char , signed int );
static void reset_pulse_buffer(void);
static void hcmd_robot_noramp(unsigned char, unsigned int );
static inline void hcmd_stop_motor(unsigned char, unsigned char);

/* Simple limit switch for the four channel input. If any
 * channel is set low than the robot is automatically stoped.
 * BUG: This should be much more sophisticated. 
 */

ISR (PCINT0_vect)
{
	if (test_bit(PINB, 0) == 0){
			PORTC &= MASK_MOTOR_A;		
			clear_bit(PORTB, 3);		// Clear bit sends a COAST to motor a

			PORTC &= MASK_MOTOR_B;		
			clear_bit(PORTB, 4);		// Clear bit sends a COAST to motor b
	}
}

/* Increments every 2ms and is cleared on each interrupt by
 * input capture. If count reaches 40ms without a clear than the
 * RC signal has been lost. */
ISR (TIMER1_COMPA_vect)
{
	if (lostPulseCount++ > 20){
		lostPulseCount = 0;					// Reset pulse counter
		reset_pulse_buffer();				// Set all pulses to neutral
		if (rcControl == TRUE)				// If in full RC mode
			 hcmd_stop_robot(COAST);		// Turn off motor drivers
	}
	OCR1A += 5000;							// 4ns count * 5000 = 2ms
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

		/* Check if pulse is in standard servo range */
		if ((pulseWidth < MAX_RC_WIDTH) && (pulseWidth > MIN_RC_WIDTH)) {

			/* Reset lost pulse counter to zero */
			lostPulseCount = 0;

			/* Check if RC drive mode */
			if ((rcSafe == TRUE) && (rcControl == TRUE)) {
	
				/* Convert pulse width (2500 to 5000) to speed values (0 ~ 625) */
				/* NOTE: 2 is used to scale the RC range */
				rcSpeed = ((signed int) pulseWidth - MID_RC_NEUTRAL) / 2;
				if (rcSpeed > +255) rcSpeed = +255;
				if (rcSpeed < -255) rcSpeed = -255;
				
				// THIS LINE IS USED TO REMOVE SPURIOUS SIGNALS CAN BE ADJUSTED 
				// if ((pulseWidth - rcPulseWidth[rcChannel]) < 200) {
				switch (rcChannel) {
					case 2:		// MOTOR B
						hcmmod_noramp(MOTORB, rcSpeed);
						break;
					case 3:		// MOTOR A
						hcmmod_noramp(MOTORA, rcSpeed);
						break;
					case 0:
						if (rcSpeed < SOLENOID_OFF_BAND) 
							clear_bit(PORTD, 3);		// On	
						else
							set_bit(PORTD, 3); 	// Off
						break;
					case 1:
						if (rcSpeed < SOLENOID_OFF_BAND) 
							clear_bit(PORTD, 4);		// On
						else
							set_bit(PORTD, 4);	// Off
						break;
				}
				//}
			}

			/* Store pulse in channel buffer */
			rcPulseWidth[rcChannel++] = pulseWidth;

			/* If safe is false, check for neutral pulse count */
			if (rcSafe == FALSE) {
				if 	(	((rcPulseWidth[0] < MAX_RC_NEUTRAL) && (rcPulseWidth[0] > MIN_RC_NEUTRAL)) &&
						((rcPulseWidth[1] < MAX_RC_NEUTRAL) && (rcPulseWidth[1] > MIN_RC_NEUTRAL)) &&
						((rcPulseWidth[2] < MAX_RC_NEUTRAL) && (rcPulseWidth[2] > MIN_RC_NEUTRAL)) &&
						((rcPulseWidth[3] < MAX_RC_NEUTRAL) && (rcPulseWidth[3] > MIN_RC_NEUTRAL)) 	) {

					if (neutralPulseCount++ > COUNT_RC_NEUTRAL) {
						rcSafe = TRUE;				// Set RC safe flag to true
						clear_bit(PORTD, 2);		// Turn on RC safe LED
					}
				}
			}

			/* Change channel */
			if (rcChannel >= 4) rcChannel = 0;
			set_rcChannel(rcChannel);
		}

		/* Set next capture for high */
		set_bit(TCCR1B, ICES1);		
	}
	sei();
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
	neutralPulseCount = 0;					// Reset neutral counter to zero
	//rcSafe = FALSE;							// Set RC safe flag to false
	set_bit(PORTD, 2);						// Turn off servo safe indicator LED
}

void hcmd_initialize_rc(char state)
{
	if (state == ENABLE) {
		TCCR1B = 	(1<<ICNC1)|			// Add noise canceller for input capture	
					(1<<ICES1)|			// Set for capture on rising edge
					(1<<CS11);			// Div 8 prescale

		TIMSK1 = 	(1<<ICIE1)|			// Set interrupt for input capture unit
					(1<<OCIE1A);		// Set interrupt for lost pulse counter

		reset_pulse_buffer();			// Reset to unsafe RC mode
	}
	else if (state == DISABLE) {
		TCCR1B &= 0xF8;					// Stop timer1 clock
		TIMSK1 = 	(0<<ICIE1)|			// Disable interrupt for input capture unit
					(0<<OCIE1A);		// Disable interrupt for lost pulse counter
	}
}

/* Initialize PWM on timer0 */
void hcmd_initialize_timers(void)
{
	/* Set timer0 pins to output */
	set_bit(DDRD, 6);
	set_bit(DDRD, 5);

	/* Set timer0 controls */
	TCCR0A = 	(1<<COM0A1)| 	// Clear OC0A on compare match, set OC0A at TOP
				(1<<COM0B1)| 	// Clear OC0B on compare match, set OC0B at TOP
				(1<<WGM01)| 	// Fast PWM
				(1<<WGM00);	

	TCCR0B = 	(1<<CS01);		// Divide by 8 prescale

	/* Initialize speed to 0 */
	OCR0A = 0;
	OCR0B = 0;
}

/* Initialize analog converter */
void hcmd_initialize_adc(void)
{
	/* Set reference and resolution for ADC */
	ADMUX = 	(1<<REFS0)| 	// Reference Voltage is VCC
				(0<<REFS1)| 
				(0<<ADLAR);		// 10 bit resolution

	/* Enable ADC and clock */
	ADCSRA = 	(1<<ADEN)| 		// Enable ADC
				(1<<ADPS2)|		// Set clock to 62.5kHz 
				(1<<ADPS1)| 
				(1<<ADPS0);	
}

/* Returns analog conversion of channel.  BUG: Should this be done in
 * interrupts? */
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

/* Set multiplexor to read specified channel */
static inline void set_rcChannel(unsigned char channel)
{
	PORTB &= 0xF9;
	PORTB |= (rcChannel << 1);
}

/* Drive motors with input from -255 to +255 */
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
		set_bit(EN2PORT, EN2BIT);	// Enable driver side B
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

/* Drive both motors with input from 0 to 255 */
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


/* Set both motors to break or coast. The effect is similar in
 * both cases, but the option may be useful in in the future. */
static inline void hcmd_stop_robot(unsigned char state)
{
	if (state == BRAKE) {
		PORTC &= 0xF0;					// Clearing direction pins breaks to ground
		set_bit(EN1PORT, EN1BIT);
		set_bit(EN2PORT, EN2BIT);  		
	}
	else if (state == COAST) {
		clear_bit(EN1PORT, EN1BIT);
		clear_bit(EN2PORT, EN2BIT);  		
	}
}

/* Set motor to break or coast. */
static inline void hcmd_stop_motor(unsigned char side, unsigned char state)
{
	if (side == MOTORA) {
		PORTC &= MASK_MOTOR_A;							// Clear direction pins
		if (state == BRAKE) set_bit(EN1PORT, EN1BIT);
		if (state == COAST) clear_bit(EN1PORT, EN1BIT);
	}
	else if (side == MOTORB) {
		PORTC &= MASK_MOTOR_B;							// Clear direction pins
		if (state == BRAKE) set_bit(EN2PORT, EN2BIT);  		
		if (state == COAST) clear_bit(EN2PORT, EN2BIT);  		
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

/* Read and average values from current feedback on motor A and B */
unsigned int hcmd_read_current(unsigned char side)
{
	unsigned int i;
	unsigned int analogValue = 0;

	if (side == MOTORA) {
		for (i = 0; i < 500; i++)
			analogValue += adc_convert(6);
		analogValue /= 500;
		return (analogValue * 100)/ 6.4;
	}
	else if (side == MOTORB) {
		for (i = 0; i < 500; i++)
			analogValue += adc_convert(7);
		analogValue /= 500;
		return (analogValue * 100)/ 6.4;
	}
	else
		return -1;
}

/* Convert speed from +-100 to +-255. Limit values to +-100 */
static signed int speed_conversion(signed int speed)
{
	if (speed > 100) speed = 100;
	if (speed < -100) speed = -100;
	return (speed * 0xFF) / 100;
}

int main(void)
{
	unsigned int current, pulse;
	signed int speed;

	/* DEBUGGING: */
	//usart_init(38400);

	rcControl = TRUE;

	hcmd_initialize_pins();
	hcmd_initialize_timers();
	hcmd_initialize_adc();
	hcmd_initialize_rc(ENABLE);

	/* Initialize both motors to neutral */
	hcmmod_noramp(MOTORA, 0); 
	hcmmod_noramp(MOTORB, 0);

	twiSlaveInit(0x04);				//Default
//	twiSlaveInit(0x05);				//Motor Module B

	sei();

	while (TRUE) {
		if (twiDataInBuffer) {
			switch (twiData[0]) {
				case CMD_MOTOR_DRIVEMOTOR:
					speed = speed_conversion((signed char) twiData[2]);
					hcmmod_noramp(twiData[1], speed);
				break;
				case CMD_MOTOR_DRIVERAW:
					speed = twiData[2] << 8;
					speed += twiData[3];
					hcmmod_noramp(twiData[1], speed);
				break;
				case CMD_MOTOR_STOPMOTOR:
					hcmd_stop_motor(twiData[1], COAST);
				break;
				case CMD_MOTOR_DRIVEROBOT:
					hcmd_robot_noramp(twiData[1], speed_conversion((signed char) twiData[2])); 
				break;
				case CMD_MOTOR_STOPROBOT:
					hcmd_stop_robot(COAST);
				break;
				case CMD_MOTOR_PNEUMATIC:
					hcmd_drive_pneumatic(twiData[1], twiData[2]);
				break;
				case CMD_MOTOR_CURRENT:
					current = hcmd_read_current(twiData[1]);
					twiData[0] = (current >> 8);
					twiData[1] = current;
				break;
				case CMD_MOTOR_PULSE:
				// IS THIS THE CORRECT CHANNEL??
					pulse =  (rcPulseWidth[(unsigned int)twiData[1]]) / 2.5;
					twiData[0] = (pulse >> 8); 
					twiData[1] = pulse; 
				break;
				case CMD_MOTOR_RCMOTOR:
					/* Convert from defines for MOTORA etc to the corresponding channels. */
					if (twiData[1] == 0) twiData[1] = 3;
					if (twiData[1] == 1) twiData[1] = 2;
					speed =  ((signed int) rcPulseWidth[(unsigned int)twiData[1]] - MID_RC_NEUTRAL) / 9;
					if (speed > +100) speed = +100;
					if (speed < -100) speed = -100;
						twiData[0] = (speed >> 8); 
						twiData[1] = speed; 
				break;
				case CMD_MOTOR_RCPNEUMATIC:
						/* Convert from defines for PNEU etc to the corresponding channels. */
						if (twiData[1] == 2) twiData[1] = 0;
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
					else if (twiData[1] == ENABLE) {
						rcControl = TRUE;
						hcmd_initialize_rc(ENABLE);
					}
				break;
				case CMD_HCMD_READ_STATE:
					twiData[0] = rcSafe;
				break;
				case CMD_MOTOR_DRIVELIMIT:
					hcmd_initialize_rc(DISABLE); 			// See BUG below	
					rcChannel = (4 - twiData[3]);
					set_rcChannel(rcChannel);
					hcmd_initialize_limitswitch(ENABLE);
					speed = speed_conversion((signed char) twiData[2]);
					hcmmod_noramp(twiData[1], speed);
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
						speed = speed_conversion((signed char) twiData[2]);
						hcmmod_noramp(twiData[1], speed);
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

