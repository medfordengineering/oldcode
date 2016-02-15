#define OW_PRT_DQ PORTC
#define OW_DIR_DQ DDRC
#define OW_VAL_DQ PINC
#define OW_PIN_DQ 0

#include <ms.h>
#include <delay.c>
#include <DS18B20.c>
#include <onewire.c>
#include <twi_master.c>
#include <twim_io.c>
#include <servo.c>

volatile unsigned int masterCounter;

ISR (TIMER0_OVF_vect)
{	
	int fridge_temp;
	int freeze_temp;

	//iomod_led(1, ON);

	/*
	if(masterCounter++ > 60) {

		fridge_temp = temp_sensor_read(0, TEMPERATURE);
		iomod_text(FIRST_LINE, "FD: ");
		fridge_temp = ((fridge_temp * 9) / 5) + 32;
		iomod_decimal(FIRST_LINE+4, fridge_temp, 2);

		freeze_temp = temp_sensor_read(3, TEMPERATURE);
		freeze_temp = ((freeze_temp * 9) / 5) + 32;
		iomod_text(SECOND_LINE, "FZ: ");
		iomod_decimal(SECOND_LINE+4, freeze_temp, 2);

		masterCounter = 0;
	}
	*/
}


int main(void)
{
	char x = 1;

	int fridge_temp;
	int freeze_temp;

	network_control(ENABLE);
	iomod_backlight(ON);

	DDRB = 0x0F;
	PORTB = 0x00;

	//TCCR0B = (5 << CS00);
	//TIMSK0 = (1 << TOIE0);
	//sei();
	//0 = defrost
	//1 = compressor

	while(1) {
/*
		fridge_temp = temp_sensor_read(3, TEMPERATURE);
		iomod_text(FIRST_LINE, "FD: ");
		fridge_temp = ((fridge_temp * 9) / 5) + 32;
		iomod_decimal(FIRST_LINE+4, fridge_temp, 2);
		*/

		freeze_temp = temp_sensor_read(0, TEMPERATURE);
		freeze_temp = ((freeze_temp * 9) / 5) + 32;
		iomod_text(SECOND_LINE, "FZ: ");
		iomod_decimal(SECOND_LINE+4, freeze_temp, 2);

		if (fridge_temp > 39)
			set_bit(PORTB, 1);
		else if (fridge_temp < 35)
			clear_bit(PORTB, 1);
			

		if (iomod_button(4) == PRESSED)
			set_bit(PORTB, 0);
		else if (iomod_button(1) == PRESSED)
			clear_bit(PORTB, 0);

		if (iomod_button(3) == PRESSED)
			set_bit(PORTB, 1);
		else if (iomod_button(2) == PRESSED)
			clear_bit(PORTB, 1);

	}
}
