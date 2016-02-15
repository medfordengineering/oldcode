//#define SMALL_BUFFER
#define TWI_MAX_BUF 30


#include <machinescience.h>
<<<<<<< test_master.c
#include <iomod.h>
=======
#include <twi_masterslave.cpp>
#include <iomod.cpp>
#include <ms_delay.cpp>

void drive_system(	unsigned char compressor, 
					unsigned char defroster, 
					unsigned char lights, 
					unsigned char damper)
{
	while(twiBusy());
	
	twiMsgSize = 5;
	twiData[0] = ((0x04 << 1) | (TWI_WRITE));
	twiData[1] = 0x02;
	twiData[2] = compressor;
	twiData[3] = defroster;
	twiData[4] = lights;
	twiData[5] = damper;

	TWCR = TWIMASTERSTART;
}

void drive_damper(unsigned int damper)
{
	while(twiBusy());
	
	twiMsgSize = 2;
	twiData[0] = ((0x04 << 1) | (TWI_WRITE));
	twiData[1] = 0x03;
	twiData[2] = damper;

	TWCR = TWIMASTERSTART;
}

void get_temperature(signed char * values)
{
	int x;
	while(twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((0x04 << 1) | (TWI_WRITE));
	twiData[1] = 0x00;
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	twiMsgSize = 4;
	twiData[0] = ((0x04 << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	for (x = 0; x < 4; x++)
		values[x] = twiData[x+1];
}

void get_door(signed char * values)
{
	int x;
	while(twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((0x04 << 1) | (TWI_WRITE));
	twiData[1] = 0x01;
	TWCR = TWIMASTERSTART;
>>>>>>> 1.59

	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((0x04 << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	for (x = 0; x < 2; x++)
		values[x] = twiData[x+1];
}
int main(void)
{
<<<<<<< test_master.c
	network_control(ENABLE);
	/*
	int y = 100;
	char x = 1;
	iomod_text(FIRST_LINE, "hi");
	while(x = 1) {
		iomod_led(1, ON);
		delay_ms(y);
		iomod_led(1, OFF);
		iomod_led(2, ON);
		delay_ms(y);
		iomod_led(2, OFF);
		iomod_led(3, ON);
		delay_ms(y);
		iomod_led(3, OFF);
		iomod_led(4, ON);
		delay_ms(y);
		iomod_led(4, OFF);
		y = y - 2;
	
	while(1)
	{
		if(iomod_button(1) == PRESSED)
		{
			iomod_led(1, ON);
			iomod_beep(A4);
		}
		if(iomod_button(2) == PRESSED)
		{
			iomod_led(2, ON);
			iomod_beep(B4);
		}
		if(iomod_button(3) == PRESSED)
		{
			iomod_led(3, ON);
			iomod_beep(C4);
		}
		if(iomod_button(4) == PRESSED)
		{
			iomod_led(4, ON);
			iomod_beep(D4);
		}
		if(iomod_button(1) == UNPRESSED)
		{
			iomod_led(1, OFF);
			iomod_beep(0);
		}
		if(iomod_button(2) == UNPRESSED)
		{
			iomod_led(2, OFF);
			iomod_beep(0);
		}
		if(iomod_button(3) == UNPRESSED)
		{
			iomod_led(3, OFF);
			iomod_beep(0);
		}
		if(iomod_button(4) == UNPRESSED)
		{
			iomod_led(4, OFF);
			iomod_beep(0);
		}
	}	
	*/
	if(imod_buttons()ss
=======
	signed char data[4];

	twiState = TWI_MASTER;
	network_control(ENABLE);

	drive_system(OFF, OFF, OFF, 50);

	while(1) {
		get_temperature(data);
		iomod_signed (FIRST_LINE, data[0], 3);
		iomod_signed (FIRST_LINE+4, data[1], 3);
		iomod_signed (SECOND_LINE, data[2], 3);
		iomod_signed (SECOND_LINE+4, data[3], 3);
		get_door(data);
	//	iomod_signed (SECOND_LINE, data[0], 3);
	//	iomod_signed (SECOND_LINE+4, data[1], 3);
	}
>>>>>>> 1.59
}
