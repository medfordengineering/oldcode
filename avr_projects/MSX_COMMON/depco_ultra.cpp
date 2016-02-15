/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Mon Jul 13 16:52:49 EDT 2009
-----------------------------------------------------------------
============================================================== */

#include <depco_ultra.h>


void set_state_ultrasonic(unsigned char state, char address)
{
	while(twiBusy());
	
	twiMsgSize = 2;
	twiData[0] = (((ULTRA_MOD_0 + address) << 1) | (TWI_WRITE));
	twiData[1] = CMD_ULTRA_SETSTATE;
	twiData[2] = state;

	TWCR = TWIMASTERSTART;
}

unsigned int read_ultrasonic(char address, char units)
{	
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = (((ULTRA_MOD_0 + address) << 1) | (TWI_WRITE));
	twiData[1] = CMD_ULTRA_CONVERT;
	twiData[2] = units;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = (((ULTRA_MOD_0 + address)<< 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return ((twiData[1] << 8) | twiData[2]);
}

unsigned int one_read_ultrasonic(char units, char address)
{	
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = (((ULTRA_MOD_0 + address) << 1) | (TWI_WRITE));
	twiData[1] = CMD_ULTRA_READ;
	twiData[2] = units;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = (((ULTRA_MOD_0 + address)<< 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return ((twiData[1] << 8) | twiData[2]);
}

void set_ultraaddress(char address, char newAddress)
{
	while (twiBusy());
	
	twiMsgSize = 2;

	twiData[0] = (((ULTRA_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_ULTRA_SETTWIADR;
	twiData[2] = ULTRA_MOD_0 + newAddress;

	TWCR = TWIMASTERSTART;
}

void display_ultraaddress(char address, char state)
{
	while (twiBusy());
	
	twiMsgSize = 2;

	twiData[0] = (((ULTRA_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_ULTRA_DISPLAYTWIADR;
	twiData[2] = state;

	TWCR = TWIMASTERSTART;
}
