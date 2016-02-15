/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue May  5 16:26:35 EDT 2009
-----------------------------------------------------------------
============================================================== */

#include <twim.h>


char keymod_press( char command )
{
	// send a command to the keypad	
	
	while(twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((KEYPAD_MODULE << 1) | (TWI_WRITE));
	twiData[1] = command;
	TWCR = TWIMASTERSTART;

	// get the value back from the keypad	

	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((KEYPAD_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	return twiData[1];
}

