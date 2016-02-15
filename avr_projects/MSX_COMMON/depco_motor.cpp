/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue May  5 16:26:35 EDT 2009
-----------------------------------------------------------------
============================================================== */

#include <depco_motor.h>

void motor_drive(unsigned char side, signed char speed)
{
	while(twiBusy());

	twiMsgSize = 3;
	
	twiData[0] = ((LCMOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LC_DRIVEMOTOR;
	twiData[2] = side;
	twiData[3] = speed; 

	TWCR = TWIMASTERSTART;
}

void drive_full(char side, signed int speed)
{
	char direction;

	while(twiBusy());

	twiMsgSize = 5;

	if (speed < 0) 
		direction = 0x02; 	// Defined in depco_motor.c
	else
		direction = 0x01;

	speed = abs(speed);
	
	twiData[0] = ((LCMOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LC_FULLDRIVE;
	twiData[2] = side;
	twiData[3] = (speed >> 8); 
	twiData[4] = speed;
	twiData[5] = direction;

	TWCR = TWIMASTERSTART;

}

unsigned char read_current(unsigned char side)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LCMOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LC_CURRENT;
	twiData[2] = side;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((LCMOTOR_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	//return ((twiData[1] << 8) | twiData[2]);
	return twiData[1];
}

void drive_robot(unsigned char direction, unsigned char speed)
{
	while(twiBusy());

	twiMsgSize = 3;
	twiData[0] = ((LCMOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LC_DRIVEROBOT;
	twiData[2] = direction;
	twiData[3] = speed;

	TWCR = TWIMASTERSTART;
}

void drive_solenoid(unsigned char state)
{
	while(twiBusy());

	twiMsgSize = 2;
	twiData[0] = ((LCMOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LC_SOLENOID;
	twiData[2] = state;

	TWCR = TWIMASTERSTART;
}

void set_limit(unsigned char channel, unsigned char state)
{
	while(twiBusy());

	twiMsgSize = 3;
	twiData[0] = ((LCMOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LC_SETLIMIT;
	twiData[2] = channel;
	twiData[3] = state;

	TWCR = TWIMASTERSTART;
}

unsigned int read_counter(unsigned char channel)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LCMOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_LC_COUNT;
	twiData[2] = channel;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LCMOTOR_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return (twiData[1] << 8) | twiData[2];
}
