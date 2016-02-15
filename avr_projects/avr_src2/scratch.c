/* --------------------------------------------------------------
   Machine Science API

   (c) Copyright Machine Science, 2008.  All rights reserved

   Version Date:   16 June 2008
-----------------------------------------------------------------
=============================================================== */

#include "scratch.h"

int value_array[8] = {0,0,0,0,0,0,0,0};

extern void scratch_send( char , int );

void send_sensor( int* value )
{
	unsigned char highbyte;
	unsigned char lowbyte;
	int count;

	// Send each of the sensor values in Scratch's proprietary format

	for (count = 0; count < 8; count++)
	{
		highbyte = ( (0x80 | ((count)<<3)) | (0x07 & (value[count]>>7)) );
		lowbyte = ( 0x7F & value[count] );
		usart_write(highbyte);
		usart_write(lowbyte);
	}

	// Send firmware id to address 15

	usart_write( (0x80 | ((0x0F)<<3)) | (0x07 & (FIRMWARE_ID>>7)) );
	usart_write(0x7F & FIRMWARE_ID);
}

void scratch_write( char port, int sensor )
{
	value_array[sensor] = adc_read(port);
}

void scratch_send( char port, int sensor )
{
	value_array[sensor] = analog_read(port);
}

/*
SIGNAL (SIG_USART_RECV)
{
	cli();
	if (UDR0 == 1)
	{
		send_sensor(value_array);
	}
	sei();
}
*/
