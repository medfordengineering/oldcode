/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

 Version Date: Wed Jun 17 10:40:59 EDT 2009
-----------------------------------------------------------------
============================================================== */
#include <xbeemod.h>

/* BUG: Check if this variable is being used. */
char DefaultTextMsgSize = 12;

void xbeemod_send_character(char character)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((XBEE_MODULE << 1) | TWI_WRITE);
	twiData[1] = CMD_XBEE_WRITE_CHARACTER;
	twiData[2] = character;
	TWCR = TWIMASTERSTART;
}

void xbeemod_configure(char * data)
{
	int i;
	while (twiBusy());
	twiMsgSize = 6;
	twiData[0] = ((XBEE_MODULE << 1) | TWI_WRITE);
	twiData[1] = CMD_XBEE_DEFAULT;
	for (i = 0; i < 5; i++)
		twiData[i + 2] = data[i];
	TWCR = TWIMASTERSTART;
}

void xbeemod_send_text(char * message)
{
	/* Checks size of message and sends a message of exactly that size along with
	 * a start byte and the length of the actuall text message. */

	int i;
	char length = strlen(message);

	while (twiBusy()) ;
	twiMsgSize = length + 2;
	twiData[0] = ((XBEE_MODULE << 1) | TWI_WRITE);
	twiData[1] = CMD_XBEE_WRITE_TEXT;
	twiData[2] = length;
	for (i = 0; i < length; i++)
		twiData[i +3] = message[i];
	TWCR = TWIMASTERSTART;
}

void xbeemod_send_decimal(unsigned int value, unsigned char place)
{
	while(twiBusy());
	twiMsgSize = 4;
	twiData[0] = ((XBEE_MODULE << 1) | TWI_WRITE);
	twiData[1] = CMD_XBEE_WRITE_DECIMAL;
	twiData[2] = value >> 8;
	twiData[3] = value;
	twiData[4] = place;
	TWCR = TWIMASTERSTART;
}

void xbeemod_receive_text(signed char *message)
{
	/* Retrieves message from the xbee mod. The first byte is the length of the
	 * message. This is used to count the message into the message buffer. A null
	 * character is added to the end of the message for display on LCD. NOTE THIS
	 * LOOKS FOR MAX_MSG_BYTES YET THEY ARE NOT SENT. THIS SEEMS TO BE WORKING BUT
	 * MIGHT BE A PROBLEM.*/

	int i;
	
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((XBEE_MODULE << 1) | TWI_WRITE);
	twiData[1] = CMD_XBEE_READ_TEXT;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = MAX_MSG_LENGTH;
	twiData[0] = ((XBEE_MODULE << 1 | TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	for (i = 0; i < twiData[1]; i++)
		message[i] = twiData[i + 2];
	message[i] = '\0';
	
}

void xbeemod_receive_joystick(signed char *message)
{
	int i;
	
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((XBEE_MODULE << 1) | TWI_WRITE);
	twiData[1] = CMD_XBEE_READ_JOY;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 6;
	twiData[0] = ((XBEE_MODULE << 1 | TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	for (i = 0; i < 6; i++)
		message[i] = twiData[i + 1];

	message[1] -= 100;
	//message[2] -= 100;
	message[2] = ((message[2] - 100) * -1);
	message[3] -= 100;
	//message[4] -= 100;
	message[4] = ((message[4] - 100) * -1);
	message[i] = '\0';
}

char xbeemod_receive_character(void)
{
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((XBEE_MODULE << 1) | TWI_WRITE);
	twiData[1] = CMD_XBEE_READ_CHARACTER;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((XBEE_MODULE << 1 | TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return twiData[1];
}

void xbeemod_send_integer(signed int value)
{
	while (twiBusy()) ;
	twiMsgSize = 3;
	twiData[0] = ((XBEE_MODULE << 1) | TWI_WRITE);
	twiData[1] = CMD_XBEE_WRITE_INTEGER;
	twiData[2] = value >> 8;
	twiData[3] = value;
	TWCR = TWIMASTERSTART;
}

char xbeemod_message_waiting(void)
{
	/* Checks to see if message is waiting in the buffer. The Xbee mod checks for a start
	 * byte in the interrupt routine and sets a flag if found, then returns a TRUE or
	 * FALSE depending on the state of the flag. The flag is cleared when text is
	 * retrieved from the buffer on the Xbee mod. */

	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((XBEE_MODULE << 1) | TWI_WRITE);
	twiData[1] = CMD_XBEE_CHECK_MESSAGE;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2; 
	twiData[0] = ((XBEE_MODULE << 1 | TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return twiData[1];
}

void xbeemod_read_configuration(char *location, char *data)  
{
	/* Reads configuration settings from any of the locations on the XBee. */

	int i;
	
	while(twiBusy());
	twiMsgSize = 3;
	twiData[0] = ((XBEE_MODULE << 1 | TWI_WRITE));
	twiData[1] = CMD_XBEE_READ_CONFIGURATION;
	twiData[2] = location[0];
	twiData[3] = location[1];
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	twiMsgSize = 9;
	twiData[0] = ((XBEE_MODULE << 1 | TWI_READ ) );
	TWCR = TWIMASTERSTART;
	
	while(twiBusy());
	for (i = 0; i < 9; i++)
		data[i] = twiData[i+1];
}

void xbeemod_write_configuration(char *location, char *data)
{
	/* Sets configuration settings on the XBee.*/

	int i;

	while(twiBusy());
	twiMsgSize = 11;
	twiData[0] = ((XBEE_MODULE << 1 | TWI_WRITE));
	twiData[1] = 0x24;
	twiData[2] = location[0];
	twiData[3] = location[1];
	//twiData[2] = location;
	for (i = 0; i < 8; i++)
		twiData[i + 4] = data[i];

	TWCR = TWIMASTERSTART;
}
/*
void xbeemod_servoinit(char command)
{
		while (twiBusy());
		twiMsgSize = 2;
		twiData[0] = ((XBEE_MODULE << 1) | TWI_WRITE);
		twiData[1] = CMD_SERVO_INIT;
		twiData[2] = command;
		
		TWCR = TWIMASTERSTART;
}

void xbeemod_servodrive(char servo, int speed)
{
		while (twiBusy());
		twiMsgSize = 4;
		twiData[0] = ((XBEE_MODULE << 1) | TWI_WRITE);
		twiData[1] = CMD_SERVO_DRIVE;
		twiData[2] = servo;
		twiData[3] = (speed >> 8);
		twiData[4] = speed;
		
		TWCR = TWIMASTERSTART;
}
*/

void xbeemod_servo_init(char state)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((XBEE_MODULE << 1) | TWI_WRITE);
	twiData[1] = 0x2E;
	twiData[2] = state;
	TWCR = TWIMASTERSTART;
}

void xbeemod_servo(char channel, unsigned int position)
{
	while (twiBusy());
	twiMsgSize = 4;
	twiData[0] = ((XBEE_MODULE << 1) | TWI_WRITE);
	twiData[1] = 0x2F;
	twiData[2] = channel;
	twiData[3] = position >> 8;
	twiData[4] = position;

	TWCR = TWIMASTERSTART;
}
