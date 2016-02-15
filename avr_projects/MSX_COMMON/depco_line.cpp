/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Mon Jul 13 16:52:49 EDT 2009
-----------------------------------------------------------------
============================================================== */

#include <depco_line.h>

unsigned char read_reflection(char address)
{	
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_WRITE));

	twiData[1] = CMD_LINE_READRANGE;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return twiData[1];
}

void set_lineaddress(char address, char newAddress)
{
	while (twiBusy());
	
	twiMsgSize = 2;

	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_LINE_SETTWIADR;
	twiData[2] = LINE_MOD_0 + newAddress;

	TWCR = TWIMASTERSTART;
}

void display_lineaddress(char address, char state)
{
	while (twiBusy());
	
	twiMsgSize = 2;

	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_LINE_DISPLAYTWIADR;
	twiData[2] = state;

	TWCR = TWIMASTERSTART;
}

void set_highpass(char address, char passValue)
{
	while (twiBusy());
	
	twiMsgSize = 2;

	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_LINE_SETHIGHPASS;
	twiData[2] = passValue;

	TWCR = TWIMASTERSTART;
}

unsigned char read_highpass(char address)
{	
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_LINE_READHIGHPASS;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return twiData[1];
}

void set_band(char address, char bandValue)
{
	while (twiBusy());
	
	twiMsgSize = 2;

	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_LINE_SETBAND;
	twiData[2] = bandValue;

	TWCR = TWIMASTERSTART;
}

unsigned char read_band(char address)
{	
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_LINE_READBAND;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return twiData[1];
}

void set_state(char address, char stateValue )
{
	while (twiBusy());
	
	twiMsgSize = 2;

	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_LINE_SETSTATE;
	twiData[2] = stateValue;

	TWCR = TWIMASTERSTART;
}

unsigned char read_pass(char address)
{	
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_LINE_READPASS;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return twiData[1];
}

void set_center(char address, char center )
{
	while (twiBusy());
	
	twiMsgSize = 2;

	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_LINE_SETCENTER;
	twiData[2] = center;

	TWCR = TWIMASTERSTART;
}

unsigned char read_center (char address)
{	
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_LINE_READCENTER;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return twiData[1];
}

unsigned int read_line_count(char address)
{	
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_WRITE));

	twiData[1] = CMD_LINE_READCAPTURE;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return ((twiData[1] << 8) | twiData[2]);
}

void set_line_count(char address, char state)
{
	while (twiBusy());
	
	twiMsgSize = 2;

	twiData[0] = (((LINE_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_LINE_SETCAPTURE;
	twiData[2] = state;

	TWCR = TWIMASTERSTART;
}
