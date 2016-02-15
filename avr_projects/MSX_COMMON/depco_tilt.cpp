/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Mon Jul 13 16:52:49 EDT 2009
-----------------------------------------------------------------
============================================================== */

#include <depco_tilt.h>

extern void gyro_setdata(unsigned char , unsigned char );
extern void gyro_writebyte(unsigned char , unsigned char);
extern void gyro_readburst(signed int * );

void tilt_init(void)
{
	axcell_init();
	gyro_init();
}

void axcell_init(void)
{
	axcell_setdata(CMD_TILT_POWER_CTL, 0x00); 	//Set standbye mode to set configuration
	axcell_setdata(CMD_TILT_BW_RATE, 0x0A); 		//Set normal power and data rate to 100Hz
	axcell_setdata(CMD_TILT_DATA_FORMAT, 0x08); 	//Set 4mg/LSB and +-2g
	axcell_setdata(CMD_TILT_POWER_CTL, 0x08); 	//Set measurement mode
}

signed int axcell_read(char axis)
{
	signed int data[4];
	signed int value;

	axcell_getdata(CMD_TILT_DATAX0, data);

	if (axis == AX_AXIS) 
		value = data[0];
	else if (axis == AY_AXIS)
		value = data[1];
	else if (axis == AZ_AXIS)
		value = data[2];

	return value;
}
	
void axcell_setdata(unsigned char command, unsigned char data)
{
    while(twiBusy());

    twiMsgSize = 2;
    twiData[0] = ((ACCELEROMETER_MODULE << 1) | (TWI_WRITE));
    twiData[1] = command;
    twiData[2] = data;

    TWCR = TWIMASTERSTART;
}

void axcell_getdata(char sensor, signed int * data)
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

void gyro_init(void)
{
    gyro_writebyte(0x3E, 0x00); //Default mode with internal clock
	gyro_writebyte(0x15, 7); //Sample rate divider set to 200Hz
    gyro_writebyte(0x16, 0x1E); //Full scale, 42Hz filter, 1kHz sample rate
    gyro_writebyte(0x17, 0x00); //Interrupt configuration?
}

void gyro_writebyte(unsigned char command, unsigned char data)
{
    while(twiBusy());

    twiMsgSize = 2;
    twiData[0] = ((GYRO_MODULE << 1) | (TWI_WRITE));
    twiData[1] = command;
    twiData[2] = data;

    TWCR = TWIMASTERSTART;
}

unsigned char gyro_readbyte(char command)
{
    while(twiBusy());
    twiMsgSize = 1;
    twiData[0] = ((GYRO_MODULE << 1) | (TWI_WRITE));
    twiData[1] = command;
    TWCR = TWIMASTERSTART;

    while(twiBusy());
    twiMsgSize = 1;
    twiData[0] = ((GYRO_MODULE << 1) | (TWI_READ));
    TWCR = TWIMASTERSTART;

    while(twiBusy());
    return twiData[1];
}

signed int gyro_read(char axis)
{
	signed int data[3];
	signed int value;

	gyro_readburst(data);

	if (axis == GX_AXIS) 
		//value = data[1] + (8 << data[2]);
		value = data[0];
	else if (axis == GY_AXIS)
		//value = data[3] + (8 << data[2]);
		value = data[1];
	else if (axis == GZ_AXIS)
		value = data[2];
		//value = data[5] + (8 << data[4]);

	return value;
}

void gyro_readburst(signed int * values)
{
    while(twiBusy());
    twiMsgSize = 1;
    twiData[0] = ((GYRO_MODULE << 1) | (TWI_WRITE));
    twiData[1] = 0x1D;
    TWCR = TWIMASTERSTART;

    while(twiBusy());
    twiMsgSize = 6;
    twiData[0] = ((GYRO_MODULE << 1) | (TWI_READ));
    TWCR = TWIMASTERSTART;

    while(twiBusy());
	values[0] = (twiData[1] << 8) | twiData[2];
	values[1] = (twiData[3] << 8) | twiData[4];
	values[2] = (twiData[5] << 8) | twiData[6];
}

