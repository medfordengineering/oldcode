/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Mon Jul 13 16:52:49 EDT 2009
-----------------------------------------------------------------
============================================================== */

#include <depco_ir.h>

unsigned int read_distance(char address)
{	
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = (((IRD_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_IRSEN_READ;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = (((IRD_MOD_0 + address)<< 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return ((twiData[1] << 8) | twiData[2]);
}

void set_iraddress(char address, char newAddress)
{
	while (twiBusy());
	
	twiMsgSize = 2;

	twiData[0] = (((IRD_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_IRSEN_SETTWIADR;
	twiData[2] = IRD_MOD_0 + newAddress;

	TWCR = TWIMASTERSTART;
}

void display_iraddress(char address, char state)
{
	while (twiBusy());
	
	twiMsgSize = 2;

	twiData[0] = (((IRD_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_IRSEN_DISPLAYTWIADR;
	twiData[2] = state;

	TWCR = TWIMASTERSTART;
}

void set_wdtstate(char address, char state)
{
	while (twiBusy());
	
	twiMsgSize = 2;

	twiData[0] = (((IRD_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_IRSEN_WDSTATE;
	twiData[2] = state;

	TWCR = TWIMASTERSTART;
}

void set_filtercount(char address, char count)
{
	while (twiBusy());
	
	twiMsgSize = 2;

	twiData[0] = (((IRD_MOD_0 + address)<< 1) | (TWI_WRITE));
	twiData[1] = CMD_IRSEN_FILTERCOUNT;
	twiData[2] = count;

	TWCR = TWIMASTERSTART;
}
