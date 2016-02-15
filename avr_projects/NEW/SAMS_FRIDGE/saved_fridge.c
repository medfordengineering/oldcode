/* Define temperature sensor ports */
#define OW_PRT_DQ PORTC
#define OW_DIR_DQ DDRC
#define OW_VAL_DQ PINC

/* Define door sensor ports */
#define DOOR_PRT	PORTB
#define DOOR_DIR	DDRB
#define DOOR_VAL	PINB

/* Define relay ports */
#define RELAY_PRT	PORTD
#define RELAY_DIR	DDRD
#define RELAY_VAL	PIND

/* Define servo ports */
#define SERVO_PRT	PORTD
#define SERVO_DIR	DDRD
#define SERVO_VAL	PIND

/* Define relay controlled devices */
#define DEFROSTER 		0
#define COMPRESSOR		1
#define LIGHTS			2

/* Define temperature sensors in fridge and freezer */
#define TOPMAIN_FREEZE_SENSE			7		
#define DEFROST_SENSE					6		
#define TOPMAIN_FRIDGE_SENSE			3
#define LOWVENT_FRIDGE_SENSE			2
#define MIDLEFT_FRIDGE_SENSE			1
#define MIDRIGHT_FRIDGE_SENSE			0

//NOTE: These should be variables
#define FRIDGE_RANGE	8
#define FREEZE_RANGE	4

//NOTE: What is this?
#define MINUTE_FACTOR	3000
#define HOUR_FACTOR		1800000L

//NOTE: These are states by might need to be variables
#define DISPLAY			0
#define SET_FRIDGE		1
#define SET_FREEZE		2

#include <ms.h>
#include <delay.c>
#include <DS18B20.c>
#include <onewire.c>
#include <twi_master.c>
#include <twim_io.c>

unsigned char systemState = DISPLAY;

signed int freezerSet = -18; 	//about 0F
signed int fridgeSet = 2; 		//about 36F
signed int defrostSet = 5; 		//about 43F

volatile unsigned long masterCounter;
volatile unsigned long servoSet;
volatile unsigned long defrostCounter;
volatile unsigned long defrostCycleSet = 6; // Time in hours between defrost cycles (change from 10)
volatile unsigned long defrostDurationSet = 20; // Time in minutes for defrost (change from 15)

void damper_control(int);
signed int temperature_convert(signed int);
void compressor(char);
void defroster(char);

/* Master Counter 
   1 =  100us 

   Defrost Counter
   1		= .02 second
   50 		= 1 second
   3000		= 1 minute
   180000	= 1 hour
 */

ISR (TIMER0_COMPA_vect)
{
	//	toggle_bit(PORTB, 0);

	/* Turn on servo pulse every 20ms */
	if (masterCounter++ > 200) {
		masterCounter = 0;
		defrostCounter++;		//Increment defrost counter every 20ms
		set_bit(PORTD, 2);
	}

	/* Turn off servo pulse */
	if (masterCounter >= servoSet)
		clear_bit(PORTD, 2);

	/* Turn on defrost cycle */
	if (defrostCounter > (defrostCycleSet * HOUR_FACTOR)) {
		defrostCounter = 0;
		compressor(OFF);
		defroster(ON);
	}

	/* Turn off defrost cycle */
	if (defrostCounter > (defrostDurationSet * MINUTE_FACTOR))
		defroster(OFF);
}

/* Convert temperature to Fahrenheit */
signed int temperature_convert(signed int temperature)
{
	return  (((temperature * 9) / 5) + 32);
}

/* Turn on/off compressor */
void compressor(char state)
{
	if (state == ON)
		set_bit(PORTB, COMPRESSOR);
	else if (state == OFF) 
		clear_bit(PORTB, COMPRESSOR);
}

/* Turn on/off defroster */
void defroster(char state)
{
	if (state == ON)
		set_bit(PORTB, DEFROSTER);
	else if (state == OFF)
		clear_bit(PORTB, DEFROSTER);
}

int main(void)
{
	signed int fridgeTemp;
	signed int freezeTemp;
	signed int defrostTemp;
	signed int fridgePro;

	network_control(ENABLE);
	iomod_backlight(ON);

	/* Initialize servo and temperature sensor ports */
	DDRB = 0x0F;
	PORTB = 0x00;
	set_bit(DDRD, 2);
	clear_bit(PORTD, 2);

	/* Set up .0001 sec timer */
	TCCR0A = (2 << WGM00);
	TCCR0B = (3 << CS00);
	TIMSK0 = (1 << OCIE0A);
	OCR0A = 31;
	sei();

	//fridgeTemp = -12;
	while(1) {

		/* Check temperature in fridge and freezer */
		fridgeTemp = temp_sensor_read(FRIDGE_SENSE, TEMPERATURE);
		freezeTemp = temp_sensor_read(FREEZE_SENSE, TEMPERATURE);
		defrostTemp = temp_sensor_read(DEFROST_SENSE, TEMPERATURE);

		/* Control freezer */
		if (test_bit(PORTB, DEFROSTER) == FALSE) {
			if (freezeTemp > (freezerSet + FREEZE_RANGE)) 
				compressor(ON);
			else if (freezeTemp < (freezerSet - FREEZE_RANGE)) 
				compressor(OFF);
		}

		/* Control refridgerator */
		//NOTE: The range is narrowed from 4 to 3
		fridgePro = ((fridgeTemp - fridgeSet) * 100) / FRIDGE_RANGE;
		if (fridgePro > 100) fridgePro = 100;
		if (fridgePro < -100) fridgePro = -100;
		servoSet = (12 + ((fridgePro * 3)/100));

		/* Defrost temperature override */
		if (defrostTemp > defrostSet)
			defroster(OFF);

		if (systemState == DISPLAY) {

			/* Display information on LCD */
			iomod_text(FIRST_LINE, "FD");
			iomod_signed(FIRST_LINE+2, fridgeTemp, 2);

			iomod_text(FIRST_LINE+6, "FZ");
			iomod_signed(FIRST_LINE+8, freezeTemp, 2);

			iomod_text(SECOND_LINE, "DF");
			iomod_signed(SECOND_LINE+2, defrostTemp, 2);

			iomod_text(SECOND_LINE+6, "D");
			iomod_decimal(SECOND_LINE+7, test_bit(PORTB, DEFROSTER), 1); 

			iomod_text(SECOND_LINE+9, "C");
			iomod_decimal(SECOND_LINE+10, test_bit(PORTB, COMPRESSOR), 1); 

			/* User interface */
			if (iomod_button(1) == PRESSED) {
				defrostCounter = (defrostCycleSet * HOUR_FACTOR);
				delay_ms(300);
			}
			if (iomod_button(4) == PRESSED) {
				systemState = SET_FRIDGE;
				delay_ms(300);
			}
		}
		else if (systemState == SET_FRIDGE) {

			iomod_text(FIRST_LINE, "FD");
			iomod_signed(FIRST_LINE+2, fridgeTemp, 2);

			iomod_text(FIRST_LINE+6, "FZ");
			iomod_signed(FIRST_LINE+8, freezeTemp, 2);

			iomod_text(SECOND_LINE, "FT");
			iomod_signed(SECOND_LINE+2, fridgeSet, 2);

			if (iomod_button(3) == PRESSED) {
				fridgeSet++;
				delay_ms(300);
			}
			if (iomod_button(2) == PRESSED) {
				fridgeSet--;
				delay_ms(300);
			}
			if (iomod_button(4) == PRESSED) {
				systemState = SET_FREEZE;
				delay_ms(300);
			}
		}
		else if (systemState == SET_FREEZE) {

			iomod_text(FIRST_LINE, "FD");
			iomod_signed(FIRST_LINE+2, fridgeTemp, 2);

			iomod_text(FIRST_LINE+6, "FZ");
			iomod_signed(FIRST_LINE+8, freezeTemp, 2);

			iomod_text(SECOND_LINE, "ZT");
			iomod_signed(SECOND_LINE+2, fridgeSet, 2);

			if (iomod_button(3) == PRESSED) {
				freezerSet++;
				delay_ms(300);
			}
			if (iomod_button(2) == PRESSED) {
				freezerSet--;
				delay_ms(300);
			}
			if (iomod_button(4) == PRESSED) {
				systemState = DISPLAY;
				delay_ms(300);
			}
		}
	}
}
