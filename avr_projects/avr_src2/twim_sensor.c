/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue May  5 16:26:35 EDT 2009
-----------------------------------------------------------------
============================================================== */

#include <sensormod.h>

#define PINPOINT_HEADER PSTR("Time,Latitude,Longitude,Acceleration,Light,Temperature,Sound,Volts,Pedometer,Altitude")

//unsigned int sensormod_data(char sensor)
signed int sensormod_data(char sensor)
{
	while(twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((SENSOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = sensor;
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((SENSOR_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	return (twiData[2] << 8) | twiData[1];
}

unsigned int sensormod_gps(char gps_unit)
{
	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((SENSOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 6;
	twiData[2] = gps_unit;
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((SENSOR_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	return (twiData[2] << 8) | twiData[1];
}

void compass_write(char command)
{
	while(twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((COMPASS_MODULE << 1) | (TWI_WRITE));
	twiData[1] = command;
	TWCR = TWIMASTERSTART;
}

unsigned int sensormod_compass(char command)
{
	while(twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((COMPASS_MODULE << 1) | (TWI_WRITE));
	twiData[1] = command;
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((COMPASS_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	return (twiData[1] << 8) | twiData[2];
}

void sensormod_initialize_gps(char state)
{
	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((SENSOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 9;
	twiData[2] = state;
	TWCR = TWIMASTERSTART;
}

void sensormod_logging(char state)
{
	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((SENSOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 7;
	twiData[2] = state;
	TWCR = TWIMASTERSTART;
}

void sensormod_upload_range(int start, int range)
{
	signed int msgBuf[21];
	int i;

	/* Headers */
	usart_const_text(PINPOINT_HEADER);
	usart_write(CARRIAGE_RETURN);
	usart_write(LINE_FEED);

	for (i = start; i < range; i++) {
		get_dataset(msgBuf, (i / 6), ((i % 6) * 42)); 
		duck_upload(msgBuf);
	}
}

void get_dataset(signed int *data, unsigned int pageAdr, unsigned int byteAdr)
{
	int i = 0;
	int j = 1;

	while(twiBusy());
	twiMsgSize = 3;
	twiData[0] = ((SENSOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 8;
	twiData[2] = pageAdr;
	twiData[3] = byteAdr;
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	twiMsgSize = 42;
	twiData[0] = ((SENSOR_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while(twiBusy());
	for (i = 0; i < 21; i ++) {
		data[i] = ((twiData[j + 1] << 8) | twiData[j]);
		j += 2;
	}
}

void duck_upload(signed int *data)
{
	unsigned long valueConversion;
	// Time and Date /
	usart_digits(data[13], BASE_10, UNSIGNED_ZEROS, 2);
	usart_write('/');
	usart_digits(data[12], BASE_10, UNSIGNED_ZEROS, 2);
	usart_write('/');
	usart_text("20");
	usart_digits(data[14], BASE_10, UNSIGNED_ZEROS, 2);
	usart_write(' ');

	usart_digits(data[0], BASE_10, UNSIGNED_ZEROS, 2);
	usart_write(':');
	usart_digits(data[1], BASE_10, UNSIGNED_ZEROS, 2);
	usart_write(':');
	usart_digits(data[2], BASE_10, UNSIGNED_ZEROS, 2);
	usart_write(',');

	// Latitude /
	usart_digits((data[3] / 100), BASE_10, SIGNED_ZEROS, 2);
	usart_write('.');
	if (data[3] < 0) data[3] *= -1;
	valueConversion = (((((unsigned long) data[3] % 100) * 10000L) + data[4]) / 60);
	usart_digits(valueConversion, BASE_10, UNSIGNED_ZEROS, 2);
	usart_write(',');

	// Longitude /
	usart_digits((data[5] / 100), BASE_10, SIGNED_ZEROS, 2);
	usart_write('.');
	if (data[5] < 0) data[5] *= -1;
	valueConversion = (((((unsigned long) data[5] % 100) * 10000L) + data[6]) / 60);
	usart_digits(valueConversion, BASE_10, UNSIGNED_ZEROS, 2);
	usart_write(',');

	// Acceleration /
	usart_digits(data[17], BASE_10, SIGNED_ZEROS, 4);
	usart_write(',');

	// Light /
	usart_digits(data[20], BASE_10, SIGNED_ZEROS, 4);
	usart_write(',');

	// Temperature /
	usart_digits(data[19], BASE_10, SIGNED_ZEROS, 4);
	usart_write(',');

	// Sound /
	usart_digits(data[15], BASE_10, SIGNED_ZEROS, 4);
	usart_write(',');

	// Voltage /
	usart_text("1.0,");

	// Pedometer /
	usart_text("1.0,");

	// Altitude /
	usart_digits(data[9], BASE_10, SIGNED_ZEROS, 5);
	usart_write(',');
	usart_write(CARRIAGE_RETURN);
	usart_write(LINE_FEED);
}

void sensormod_set_usart(unsigned int baud)
{
	while(twiBusy());
	twiMsgSize = 3;
	twiData[0] = ((SENSOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 10;
	twiData[2] = (baud >> 8);
	twiData[3] = baud;
	TWCR = TWIMASTERSTART;
}

void sensormod_gpsinit(void)
{
	while(twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((SENSOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 11;
	TWCR = TWIMASTERSTART;
}
