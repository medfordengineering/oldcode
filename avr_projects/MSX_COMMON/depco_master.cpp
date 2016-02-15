/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Mon Jul 13 16:52:49 EDT 2009
-----------------------------------------------------------------
============================================================== */

#include <depco_master.h>

/* Converts 0 - 255 to -100 - +100 */
signed char convert_values(unsigned char value)
{
  value = ((unsigned int)value * 200)/0xFF;
  return (signed char)value - 100;
}

void display_character( unsigned char command, unsigned char character)
{
	while(twiBusy());
	
	twiMsgSize = 3;
	twiData[0] = ((PSLCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_PSL_CHAR;
	twiData[2] = command;
	twiData[3] = character;

	TWCR = TWIMASTERSTART;
}

void display_text( unsigned char command,  char * text)
{
	unsigned char i;
	i = strlen(text);

	while(twiBusy());
	twiMsgSize = (2 + i);
	twiData[0] = ((PSLCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_PSL_TEXT;
	twiData[2] = command;
	i = 3;
	while( (*text != '\0') && (i < TWI_MAX_BUF) ) {
		twiData[i++] = *text;
		text++;
	}
	TWCR = TWIMASTERSTART;
}

void display_signed( unsigned char command, signed int value, unsigned char digits)
{
	
	while(twiBusy());
	twiMsgSize = 5;
	twiData[0] = ((PSLCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_PSL_PRINT_SIGNED;
	twiData[2] = command;
	twiData[3] = (value >> 8);
	twiData[4] = value;
	twiData[5] = digits;

	TWCR = TWIMASTERSTART;
}

void display_decimal( unsigned char command, unsigned int value, unsigned char digits)
{
	while(twiBusy());
	twiMsgSize = 5;
	twiData[0] = ((PSLCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_PSL_PRINT_DEC;
	twiData[2] = command;
	twiData[3] = (value >> 8);
	twiData[4] = value;
	twiData[5] = digits;

	TWCR = TWIMASTERSTART;
}

void display_hexadecimal( unsigned char command, unsigned int value, unsigned char digits)
{
	while(twiBusy());
	twiMsgSize = 5;
	twiData[0] = ((PSLCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_PSL_PRINT_HEX;
	twiData[2] = command;
	twiData[3] = (value >> 8);
	twiData[4] = value;
	twiData[5] = digits;

	TWCR = TWIMASTERSTART;
}

void display_binary (unsigned char command, unsigned int value, unsigned char digits)
{
	while(twiBusy());
	twiMsgSize = 5;
	twiData[0] = ((PSLCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_PSL_PRINT_BIN;
	twiData[2] = command;
	twiData[3] = (value >> 8);
	twiData[4] = value;
	twiData[5] = digits;

	TWCR = TWIMASTERSTART;
}

void display_bar (unsigned char line, unsigned char value)
{

	while(twiBusy());
	twiMsgSize = 3;
	twiData[0] = ((PSLCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_PSL_BAR;
	twiData[2] = line;
	twiData[3] = value;
	
	TWCR = TWIMASTERSTART;
}

void display_clear (void)
{
	while(twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((PSLCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_PSL_CLEAR;

	TWCR = TWIMASTERSTART;
}

void beep_speaker(unsigned int frequency)
{
	while(twiBusy());
	twiMsgSize = 3;
	twiData[0] = ((PSLCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_PSL_BEEP;
	twiData[2] = frequency >> 8;
	twiData[3] = frequency; 

	TWCR = TWIMASTERSTART;
}

void display_backlight(unsigned char command)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((PSLCD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_PSL_BACKLIGHT;
	twiData[2] = command;

	TWCR = TWIMASTERSTART;
}

void ps2_override(unsigned char state)
{
	if (state == ENABLE) {
		while (twiBusy());
		twiMsgSize = 1;
		twiData[0] = ((PSLCD_MODULE << 1) | (TWI_WRITE));
		twiData[1] = CMD_PSL_OVERRIDE;

		TWCR = TWIMASTERSTART;
	}
}

void depco_control(unsigned char state)
{
	twiState = TWI_MASTER; 
	network_control(ENABLE);
	ps2_override(ENABLE);
}

signed char ps2_joystick(unsigned char stick)
{	
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((PSLCD_MODULE<< 1) | (TWI_WRITE));
	twiData[1] = CMD_PSL_ANALOG;
	twiData[2] = stick;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((PSLCD_MODULE<< 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return (signed char) twiData[1];
}

unsigned char ps2_joystick_fullrange(unsigned char stick)
{	
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((PSLCD_MODULE<< 1) | (TWI_WRITE));
	twiData[1] = CMD_PSL_ANARAW;
	twiData[2] = stick;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((PSLCD_MODULE<< 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return twiData[1];
}

unsigned char ps2_button(unsigned int button)
{	
	while (twiBusy());
	twiMsgSize = 3;
	twiData[0] = ((PSLCD_MODULE<< 1) | (TWI_WRITE));
	twiData[1] = CMD_PSL_BUTTON;
	twiData[2] = button >> 8;
	twiData[3] = button;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((PSLCD_MODULE<< 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return twiData[1];
}

void start_delay(unsigned char delay)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((PSLCD_MODULE<< 1) | (TWI_WRITE));
	twiData[1] = CMD_PSL_STARTDELAY;
	twiData[2] = delay;
	TWCR = TWIMASTERSTART;
}


