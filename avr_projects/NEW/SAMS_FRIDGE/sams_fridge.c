/* Define temperature sensor ports */
#define OW_PRT_DQ PORTD
#define OW_DIR_DQ DDRD
#define OW_VAL_DQ PIND

/* Define door sensor ports */
#define DOOR_PRT	PORTB
#define DOOR_DIR	DDRB
#define DOOR_VAL	PINB

/* Define relay ports */
#define RELAY_PRT	PORTC
#define RELAY_DIR	DDRC
#define RELAY_VAL	PINC

/* Define servo ports */
#define SERVO_PRT	PORTB
#define SERVO_DIR	DDRB
#define SERVO_VAL	PINB

/* Define relay pins */
#define DEFROSTER 						0
#define COMPRESSOR						1
#define LIGHTS							2

/* Define temperature sensor pins */
#define TOPMAIN_FREEZE_SENSE			0		
#define DEFROST_SENSE					1		
#define TOPMAIN_FRIDGE_SENSE			2
#define LOWVENT_FRIDGE_SENSE			3
#define MIDLEFT_FRIDGE_SENSE			4
#define MIDRIGHT_FRIDGE_SENSE			5

/* Define servo pin */
#define FRIDGE_DAMPER 					1

/* Define door sensor pins */
#define FRIDGE_DOOR						4
#define FREEZE_DOOR						5

/* Door states */
#define CLOSED							0
#define OPEN							1

/* Define drive states for compressor, damper, lights, defrost */
#define AUTOMATIC						2

#include <ms.h>
#include <delay.c>
#include <DS18B20.c>
#include <onewire.c>
#include <twi_slave.c>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <sams_fridge.h>

/* Temperatures */
volatile signed char freezeTemp; 
volatile signed char defrostTemp;
volatile signed char fridgeTemp;
volatile signed char lowventTemp;

/* Temperature set points */
volatile signed char freezeSet;
volatile signed char fridgeSet;
volatile signed char defrostSet;

/* Temperature ranges */
volatile signed char freezeRange;
volatile signed char fridgeRange;

/* Fridge and freezer door states */
volatile unsigned char freezeDoor;
volatile unsigned char fridgeDoor;

/* System states define by master */
volatile unsigned char systemState;

/* Updates controls every ~33ms */
ISR (TIMER0_OVF_vect)
{
	/* Update temperature from all sensors */
	freezeTemp = temp_sensor_read(TOPMAIN_FREEZE_SENSE, TEMPERATURE);
	fridgeTemp = temp_sensor_read(TOPMAIN_FRIDGE_SENSE, TEMPERATURE);
	defrostTemp = temp_sensor_read(DEFROST_SENSE, TEMPERATURE);
	lowventTemp = temp_sensor_read(LOWVENT_FRIDGE_SENSE, TEMPERATURE);

	/* Update state of doors */ 
	freezeDoor = test_bit(DOOR_VAL, FREEZE_DOOR);
	fridgeDoor = test_bit(DOOR_VAL, FRIDGE_DOOR);
	
	/* Drive system */
	//drive_system(systemStat);
}

void initialize_generalports(void)
{
	/* Set relay control pins as outputs */
	set_bit(RELAY_DIR, DEFROSTER);
	set_bit(RELAY_DIR, COMPRESSOR);
	set_bit(RELAY_DIR, LIGHTS);

	/* Set servo control pin as output */
	set_bit(SERVO_DIR, FRIDGE_DAMPER);

	/* Set door sensor pins as inputs */
	clear_bit(DOOR_DIR, FRIDGE_DOOR);
	clear_bit(DOOR_DIR, FREEZE_DOOR);
}

/* Initialize for 50hz with a 750 OCR1 neutral point */
void initialize_servocontrol(char state)
{
	if (state == ENABLE) {
		TCCR1A = 	(2<<COM1A0);	// Clear OC1A on up count and set on down count
		TCCR1B = 	(1<<WGM13) |	// Mode = PWM phase and frequency correct with ICR1 as TOP
					(2<<CS10);		// Set prescale to divide by 8

		ICR1 = 10000;				// Set for 50Hz	

		OCR1A = 750;				// Neutral pulse of 1500ms
	}
	else if (state == DISABLE) {
		TCCR1B = 	(0<<CS10);		// Turn off timer1
		TCCR1A = 0x00;				// Disconnnect OC1A/B and mode = normal
	}
}

/* Set master timer to interrupt every ~ 33ms */
void initialize_mastertimer(char state)
{
	if (state == ENABLE) {
		TCCR0A = 0x00;				// Normal operation 
		TCCR0B = (5<<CS00); 		// Set prescale to divide by 1024
		TIMSK0 = (1<<TOIE0);			// Set interrupt for overflow
	}
	else if (state == DISABLE) {
		TCCR0B = 0x00;				// Turn off timer0
		TIMSK0 = 0x00;				// Clear all interrupts
	}
}

/* Overall system drive with automatic control */
void drive_system(char state)
{
	drive_damper(test_bit(state, 0));
	drive_compressor(test_bit(state, 1));
	drive_defroster(test_bit(state, 2));
	drive_lights(test_bit(state, 3));
}

/* Drives damper either proportionally or full off */
void drive_damper (char state)
{
	
	if (state == AUTOMATIC)
		set_damper(((fridgeTemp - fridgeSet) * 100) / fridgeRange);
	else if (state == ON)
		set_damper(-100);
	else if (state == OFF)
		set_damper(+100);
}

/* Sets servo value directly */
void set_damper(signed int proportion)
{
	if (proportion > 100) proportion = 100; 		// Avoids overshoot
	if (proportion < -100) proportion = -100;		// Avoids undershoot
	proportion = (proportion + 100) * 5; 			// Converts -100/100 to 500/1000
	OCR1A = proportion;								// Sets servo position
}

/* Drives compressor either based on feedback or off */
void drive_compressor(char state)
{
	if (state == AUTOMATIC) {
		/* Turn on compresser if over temperature limit */
		if (freezeTemp > (freezeSet + freezeRange))
			set_compressor(ON);
		/* Turn off compressor if under temperature limit */
		else if (freezeTemp < (freezeSet - freezeRange))
			set_compressor(OFF);
	}
	else 
		set_compressor(state);
}

/* Sets compressor state directly */
void set_compressor(char state)
{
	if (state == ON)
		set_bit(RELAY_PRT, COMPRESSOR);
	else if (state == OFF)
		set_bit(RELAY_PRT, COMPRESSOR);
}

/* Turn on/off defroster */
void drive_defroster(char state)
{
	if (state == ON)
		set_bit(RELAY_PRT, DEFROSTER);
	else if (state == OFF)
		clear_bit(RELAY_PRT, DEFROSTER);
}

/* Drives lights either based on feedback or off */
void drive_lights(char state)
{
	if (state == AUTOMATIC) {
		if (freezeDoor == OPEN)
			set_lights(ON);
		else if (freezeDoor == CLOSED)
			set_lights(OFF);
	}
	else 
		set_lights (state);
}

void set_lights (char state) 
{
	if (state == ON) 
		set_bit(RELAY_PRT, LIGHTS);
	else if (state == OFF)
		clear_bit(RELAY_PRT, LIGHTS);
}

//WDT
int main(void)
{
	initialize_generalports();
	initialize_mastertimer(ENABLE);
	sei();
	//Wait for info from MASTER to set freezerSet, fridgeSet, and defrostSet

	twiSlaveInit(0x04);

	while(TRUE) {
		if (twiDataInBuffer) {
			switch (twiData[0]) {
				case CMD_GET_TEMPS:
					twiData[0] = freezeTemp;
					twiData[1] = fridgeTemp;
					twiData[2] = defrostTemp;
					twiData[3] = lowventTemp;
				break;
				case CMD_GET_DOOR:
					twiData[0] = freezeDoor;
					twiData[0] = fridgeDoor;
				break;
				case CMD_DRIVE_COMPRESS:
					drive_compressor(twiData[1]);
				break;
				case CMD_DRIVE_DEFROST:
					drive_defroster(twiData[1]);
				break;
				case CMD_DRIVE_DAMPER:
					drive_damper(twiData[1]);
				break;
				case CMD_DRIVE_LIGHTS:
					drive_lights(twiData[1]);
				break;
				case CMD_SET_POINTS:
				//	drive_compressor(twiData[1]);
				break;
			}
			TWCR = TWISLAVEACTIVEACK;
			twiDataInBuffer = FALSE;
		}
	}
}
					
		


