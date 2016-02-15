#define OW_PRT_DQ PORTC
#define OW_DIR_DQ DDRC
#define OW_VAL_DQ PINC

#define DEFROST 		0
#define COMPRESSOR		1
#define FRIDGE_SENSE	3
#define FREEZE_SENSE	0		
#define DEFROST_SENSE	1		

#define FRIDGE_RANGE	10
#define FREEZE_RANGE	5

#include <ms.h>
#include <delay.c>
#include <DS18B20.c>
#include <onewire.c>
#include <twi_master.c>
#include <twim_io.c>

signed int freezerSet = -10; 	//about 14F
signed int fridgeSet = 2; 		//about 36F

volatile unsigned int masterCounter;

/* .01 sec. interrupt */
ISR (TIMER0_COMPA_vect)
{
	if (masterCounter++ > 100) {
		masterCounter = 0;
	}
}

/* Servo control of damper */
void damper_control(int position)
{
	char x;

	clear_bit(PORTD, 2);
	for (x = 0; x < 20; x++) {
		set_bit(PORTD, 2);
		delay_us(position);
		clear_bit(PORTD, 2);
		delay_ms(20);
	}
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
		set_bit(PORTB, DEFROST);
	else if (state == OFF)
		clear_bit(PORTB, DEFROST);
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

	/* Set up .01 sec timer */
	TCCR0A = (2 << WGM00);
	TCCR0B = (5 << CS00);
	TIMSK0 = (1 << OCIE0A);
	OCR0A = 195;
	sei();

	//fridgeTemp = -5;
	//freezeTemp = -18;

	while(1) {

		/* Check tempreture in fridge and freezer */
		fridgeTemp = temp_sensor_read(FRIDGE_SENSE, TEMPERATURE);
		freezeTemp = temp_sensor_read(FREEZE_SENSE, TEMPERATURE);
//		defrostTemp = temp_sensor_read(DEFROST_SENSE, TEMPERATURE);

		/* Control freezer */
		if (freezeTemp > (freezerSet + FREEZE_RANGE)) 
			compressor(ON);
		else if (freezeTemp < (freezerSet - FREEZE_RANGE)) 
			compressor(OFF);

		/* Control refridgerator */
		fridgePro = ((fridgeTemp - fridgeSet) * 100) / FRIDGE_RANGE;
		if (fridgePro > 100) fridgePro = 100;
		if (fridgePro < -100) fridgePro = -100;
		damper_control(1200 + (fridgePro * 4));

		/* Display information on LCD */
		iomod_text(FIRST_LINE, "FD:");
		iomod_signed(FIRST_LINE+3, fridgeTemp, 2);

		iomod_text(SECOND_LINE, "FZ:");
		iomod_signed(SECOND_LINE+3, freezeTemp, 2);

	//	iomod_text(SECOND_LINE+6, "FZ:");
	//	iomod_signed(SECOND_LINE+9, defrostTemp, 2);
	}
}
