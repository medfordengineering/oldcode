#include <accelerometer.h>
/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2012.  All rights reserved

Version Date: Thu Feb  9 14:14:16 EST 2012

-----------------------------------------------------------------
============================================================== */


void xcell_setdata(unsigned char command, unsigned char data)
{
    while(twiBusy());

    twiMsgSize = 2;
    twiData[0] = ((ACCELEROMETER_MODULE << 1) | (TWI_WRITE));
    twiData[1] = command;
    twiData[2] = data;

    TWCR = TWIMASTERSTART;
}

void xcell_getdata(char sensor, signed int * data)
{
    while(twiBusy());
    twiMsgSize = 1;
    twiData[0] = ((ACCELEROMETER_MODULE << 1) | (TWI_WRITE));
    twiData[1] = sensor;
    TWCR = TWIMASTERSTART;

    while(twiBusy());
    twiMsgSize = 6;
    twiData[0] = ((ACCELEROMETER_MODULE << 1) | (TWI_READ));
    TWCR = TWIMASTERSTART;

    while(twiBusy());
	data[0]  = (twiData[2] << 8) | twiData[1];
	data[1]  = (twiData[4] << 8) | twiData[3];
	data[2]  = (twiData[6] << 8) | twiData[5];
}

void accelerometer_init()
{
	network_control(ENABLE);

	/* Initialize accelerometer */
	xcell_setdata(0x2D, 0x00); //Put in standbye mode to set configuration
	xcell_setdata(0x2C, 0x0A); //Set normal power and data rate to 100Hz
	xcell_setdata(0x31, 0x0B); //Set full resolution +-16g
	xcell_setdata(0x2D, 0x08); //Put in measurement mode
}

signed int accelerometer_read(unsigned char data)
{
	signed int values[3];
	xcell_getdata(0x32, values);
	return values[(data-1)];
}

