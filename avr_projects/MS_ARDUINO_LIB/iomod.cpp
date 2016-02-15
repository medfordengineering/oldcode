/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Mon Jul 13 16:52:49 EDT 2009
-----------------------------------------------------------------
============================================================== */

#include <iomod.h>

unsigned int iomod_calibrate_system_clock(void)
{
	
	while(twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_IO_CALIBRATE_RC;
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	return (twiData[1] << 8) + twiData[2];
}

void iomod_character( unsigned char command, unsigned char character)
{
	while(twiBusy());
	
	twiMsgSize = 3;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LCD_CHAR;
	twiData[2] = command;
	twiData[3] = character;

	TWCR = TWIMASTERSTART;
}

void iomod_text( unsigned char command,  char * text)
{
	unsigned char i;
	i = strlen(text);

	while(twiBusy());
	twiMsgSize = (2 + i);
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LCD_TEXT;
	twiData[2] = command;
	i = 3;
	while( (*text != '\0') && (i < TWI_MAX_BUF) ) {
		twiData[i++] = *text;
		text++;
	}
	TWCR = TWIMASTERSTART;
}

void iomod_signed( unsigned char command, signed int value, unsigned char digits)
{
	
	while(twiBusy());
	twiMsgSize = 5;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_IO_PRINT_SIGNED;
	twiData[2] = command;
	twiData[3] = (value >> 8);
	twiData[4] = value;
	twiData[5] = digits;

	TWCR = TWIMASTERSTART;
}

void iomod_decimal( unsigned char command, unsigned int value, unsigned char digits)
{
	while(twiBusy());
	twiMsgSize = 5;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LCD_PRINT_DEC;
	twiData[2] = command;
	twiData[3] = (value >> 8);
	twiData[4] = value;
	twiData[5] = digits;

	TWCR = TWIMASTERSTART;
}

void iomod_hexadecimal( unsigned char command, unsigned int value, unsigned char digits)
{
	while(twiBusy());
	twiMsgSize = 5;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LCD_PRINT_HEX;
	twiData[2] = command;
	twiData[3] = (value >> 8);
	twiData[4] = value;
	twiData[5] = digits;

	TWCR = TWIMASTERSTART;
}

void iomod_binary (unsigned char command, unsigned int value, unsigned char digits)
{
	while(twiBusy());
	twiMsgSize = 5;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LCD_PRINT_BIN;
	twiData[2] = command;
	twiData[3] = (value >> 8);
	twiData[4] = value;
	twiData[5] = digits;

	TWCR = TWIMASTERSTART;
}

void iomod_bar (unsigned char line, unsigned char value)
{
	int i;

	while(twiBusy());
	twiMsgSize = 18;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LCD_BAR;
	twiData[2] = line;
	
	for (i = 0; i < value; i++)
		twiData[i + 3] = 0xff;
	for (; i < 16; i++)
		twiData[i + 3] = ' ';
	
	TWCR = TWIMASTERSTART;
}

void iomod_clear (void)
{
	while(twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LCD_CLEAR;

	TWCR = TWIMASTERSTART;
}

signed char iomod_remote(void)
{
	
	while(twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LCD_REMOTE;
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	return twiData[1];
}

void iomod_beep(unsigned int frequency)
{
	while(twiBusy());
	twiMsgSize = 3;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LCD_BEEP;
	twiData[2] = frequency >> 8;
	twiData[3] = frequency; 

	TWCR = TWIMASTERSTART;
}

void iomod_song(unsigned int * songBuffer, unsigned char notes)
{
	int i, j = 3;

	while(twiBusy());
	twiMsgSize = (((int)notes * 4) + 2);
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_IO_TONE;
	twiData[2] = notes;

	for (i = 0; i < (notes * 2); i++) {
		twiData[j++] = songBuffer[i] >> 8;
		twiData[j++] = songBuffer[i];
	}

	TWCR = TWIMASTERSTART;
}

void iomod_led(unsigned char led, unsigned char command)
{
	while(twiBusy());
	twiMsgSize = 3;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LCD_LED;
	twiData[2] = command;
	twiData[3] = led;

	TWCR = TWIMASTERSTART;
}

void iomod_leds(unsigned char led)
{
	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_IO_LED_ALL;
	twiData[2] = led;

	TWCR = TWIMASTERSTART;
}

unsigned char iomod_button( unsigned char command )
{
	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LCD_BTN;
	twiData[2] = command;
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	return twiData[1];
}

unsigned char iomod_buttons(void)
{
	
	while(twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_IO_BTN_ALL;
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	return twiData[1];
}

unsigned char iomod_time(unsigned char units)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LCD_TIME;
	twiData[2] = units;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return twiData[1];
}

unsigned char iomod_date(unsigned char units)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LCD_DATE;
	twiData[2] = units;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return twiData[1];
}

void iomod_set_time(
						unsigned char hours, 
						unsigned char minutes, 
						unsigned char seconds,
						unsigned char ampm
					)	
{
	while (twiBusy());
	twiMsgSize = 5;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LCD_SET_TIME;
	twiData[2] = hours;
	twiData[3] = minutes;
	twiData[4] = seconds;
	twiData[5] = ampm;

	TWCR = TWIMASTERSTART;
}
		
void iomod_set_date(
						unsigned char month, 
						unsigned char day, 
						unsigned char year
					)	
{
	while (twiBusy());
	twiMsgSize = 4;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LCD_SET_DATE;
	twiData[2] = month;
	twiData[3] = day;
	twiData[4] = year;

	TWCR = TWIMASTERSTART;
}

void iomod_display_time(unsigned char command)
{
	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_IO_DISPLAY_TIME;
	twiData[2] = command;

	TWCR = TWIMASTERSTART;
}

void iomod_display_date(unsigned char command)
{
	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_IO_DISPLAY_DATE;
	twiData[2] = command;

	TWCR = TWIMASTERSTART;
}

void iomod_backlight(unsigned char command)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LCD_BACKLIGHT;
	twiData[2] = command;

	TWCR = TWIMASTERSTART;
}
