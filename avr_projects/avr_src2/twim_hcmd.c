/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue May  5 16:26:35 EDT 2009
-----------------------------------------------------------------
============================================================== */

#include <hcmmod.h>

void hcmmod_motor(unsigned char side, signed char speed)
{
	while(twiBusy());

	twiMsgSize = 3;
	
	// For testing multiple modules. This needs to be cleaned up.
	if (side == MOTORC) {
		twiData[0] = ((MOTOR_MODULEA << 1) | (TWI_WRITE));
		side = MOTORA;
	}
	else if (side == MOTORD) {
		twiData[0] = ((MOTOR_MODULEA << 1) | (TWI_WRITE));
		side = MOTORB;
	}
	else
		twiData[0] = ((MOTOR_MODULE << 1) | (TWI_WRITE));

	twiData[1] = CMD_MOTOR_DRIVEMOTOR;
	twiData[2] = side;
	twiData[3] = speed; 

	TWCR = TWIMASTERSTART;
}

void hcmmod_motor_8bit(unsigned char side, signed int speed)
{
	while(twiBusy());
	twiMsgSize = 4;
	
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_WRITE));

	twiData[1] = 0x53;
	twiData[2] = side;
	twiData[3] = (speed >> 8); 
	twiData[4] = speed;

	TWCR = TWIMASTERSTART;
}

void hcmd_stop_motor(unsigned char side, unsigned char state)
{
	while(twiBusy());

	twiMsgSize = 3;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MOTOR_STOPMOTOR;
	twiData[2] = side;
	twiData[3] = state;

	TWCR = TWIMASTERSTART;
}

void hcmmod_robot(unsigned char direction, unsigned char speed)
{
	while(twiBusy());

	twiMsgSize = 3;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MOTOR_DRIVEROBOT;
	twiData[2] = direction;
	twiData[3] = speed;

	TWCR = TWIMASTERSTART;
}

void hcmd_stop_robot(unsigned char state)
{
	while(twiBusy());

	twiMsgSize = 2;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MOTOR_STOPROBOT;
	twiData[2] = state;

	TWCR = TWIMASTERSTART;
}

void hcmmod_solenoid(unsigned char side, unsigned char state)
{
	while(twiBusy());

	// For testing multiple modules. This needs to be cleaned up.
	twiMsgSize = 3;
	if (side == SOLENOIDC) { 
		twiData[0] = ((MOTOR_MODULEA << 1) | (TWI_WRITE));
		side = SOLENOIDB;
	}
	else 
		twiData[0] = ((MOTOR_MODULE << 1) | (TWI_WRITE));

	twiData[1] = CMD_MOTOR_PNEUMATIC;
	twiData[2] = side;
	twiData[3] = state;

	TWCR = TWIMASTERSTART;
}

void hcmd_drive_limit(unsigned char side, signed char speed, unsigned char channel)
{
	while(twiBusy());

	twiMsgSize = 4;

	// For testing multiple modules. This needs to be cleaned up.
	if (side == MOTORC) {
		twiData[0] = ((MOTOR_MODULEA << 1) | (TWI_WRITE));
		side = MOTORA;
	}
	else if (side == MOTORD) {
		twiData[0] = ((MOTOR_MODULEA << 1) | (TWI_WRITE));
		side = MOTORB;
	}
	else
		twiData[0] = ((MOTOR_MODULE << 1) | (TWI_WRITE));

	twiData[1] = CMD_MOTOR_DRIVELIMIT;
	twiData[2] = side;
	twiData[3] = speed;
	twiData[4] = channel;

	TWCR = TWIMASTERSTART;
}

void hcmd_drive_block(unsigned char side, signed char speed, unsigned char channel)
{
	while(twiBusy());

	twiMsgSize = 4;
	if (side == MOTORC) {
		twiData[0] = ((MOTOR_MODULEA << 1) | (TWI_WRITE));
		side = MOTORA;
	}
	else if (side == MOTORD) {
		twiData[0] = ((MOTOR_MODULEA << 1) | (TWI_WRITE));
		side = MOTORB;
	}

	twiData[1] = CMD_MOTOR_DRIVEBLOCK;
	twiData[2] = side;
	twiData[3] = speed;
	twiData[4] = channel;

	TWCR = TWIMASTERSTART;
}

unsigned int hcmmod_current(unsigned char side)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MOTOR_CURRENT;
	twiData[2] = side;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return ((twiData[1] << 8) | twiData[2]);
}

//unsigned int hcmd_read_pulse(unsigned char channel)
unsigned int hcmmod_channel(unsigned char channel)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MOTOR_PULSE;
	twiData[2] = channel;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return (twiData[1] << 8) | twiData[2];
}

unsigned int hcmd_read_rcmotor(unsigned char channel)
{

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MOTOR_RCMOTOR;
	twiData[2] = channel;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return (twiData[1] << 8) | twiData[2];
}

unsigned int hcmd_read_rcpneumatic(unsigned char channel)
{

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MOTOR_RCPNEUMATIC;
	twiData[2] = channel;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return twiData[1];
}

void hcmmod_rc(unsigned char state)
{

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_HCMD_WRITE_STATE;
	twiData[2] = state;

	TWCR = TWIMASTERSTART;
}

void hcmmod_rc_multi(unsigned char state, unsigned char address)
{

	while (twiBusy());
	twiMsgSize = 2;
	if (address == 0)
		twiData[0] = ((MOTOR_MODULE << 1) | (TWI_WRITE));
	else if (address == 1)
		twiData[0] = ((MOTOR_MODULEA << 1) | (TWI_WRITE));
	twiData[1] = CMD_HCMD_WRITE_STATE;
	twiData[2] = state;

	TWCR = TWIMASTERSTART;
}
	
unsigned char hcmd_read_rcstate(void)
{

	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_HCMD_READ_STATE;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return twiData[1];
}

void hcmmod_ramping(unsigned char state)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_HCMD_RAMPING;
	twiData[2] = state;

	TWCR = TWIMASTERSTART;
}

unsigned int hcmmod_read_counter(unsigned char channel)
{

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_PULSE_COUNTER;
	twiData[2] = channel;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MOTOR_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return (twiData[1] << 8) | twiData[2];
}
