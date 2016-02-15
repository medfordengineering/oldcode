/* -------------------------------------------------------------
 Machine Science API
 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue May  5 16:29:15 EDT 2009
 
-----------------------------------------------------------------
============================================================== */
#ifndef _TWIM_HCMD_H_
#define _TWIM_HCMD_H_

#include <twi_master.h>
#include <twim_util.h>
#include <adc.h>
#include <timers.h>
#include <delay.h>

#define FORWARD 				0x00
#define REVERSE					0x01
#define SPIN_RIGHT				0x02
#define SPIN_LEFT 				0x03
#define MOTORA					0x00
#define MOTORB					0x01
#define MOTORC					0x02
#define MOTORD					0x03
#define SOLENOIDA				0x02
#define SOLENOIDB				0x03
#define SOLENOIDC				0x04
#define SOLENOIDD				0x05
#define COAST					0x00
#define BRAKE					0x01
#define STOP					0x04		

#define CHANNEL1				0x00
#define CHANNEL2				0x01
#define CHANNEL3				0x02
#define CHANNEL4				0x03

#define CMD_MOTOR_DRIVEMOTOR	0x37
#define CMD_MOTOR_PNEUMATIC		0x38
#define CMD_MOTOR_CURRENT		0x39
#define CMD_MOTOR_ISRCSIG		0x3A
#define CMD_MOTOR_READRCSIG		0x3B
#define CMD_MOTOR_DRIVEROBOT	0x48
#define CMD_MOTOR_STOPROBOT		0x49
#define CMD_MOTOR_STOPMOTOR		0x4A
#define CMD_MOTOR_PULSE			0x4B
#define CMD_MOTOR_RCMOTOR		0x4C
#define CMD_MOTOR_RCPNEUMATIC 	0x4D
#define CMD_HCMD_READ_STATE		0x4E
#define CMD_HCMD_WRITE_STATE	0x4F
#define CMD_MOTOR_DRIVELIMIT	0x50
#define CMD_HCMD_RAMPING		0x51
#define CMD_MOTOR_DRIVEBLOCK	0x52
#define CMD_MOTOR_DRIVERAW		0x53
#define CMD_PULSE_COUNTER		0x54

#ifndef MONITOR
	#define MONITOR					0x02
#endif

extern void hcmmod_motor(unsigned char, signed char);
extern void hcmmod_rc(unsigned char);
extern void hcmmod_solenoid(unsigned char, unsigned char);
extern unsigned int hcmmod_current(unsigned char);
extern void hcmmod_robot(unsigned char, unsigned char);
extern void hcmmod_rc_multi(unsigned char, unsigned char);
extern void hcmd_drive_robot(unsigned char, unsigned int);
extern void hcmd_stop_robot(unsigned char );
//extern unsigned int hcmd_read_pulse(unsigned char );
extern unsigned int hcmmod_channel(unsigned char );
extern unsigned int hcmd_read_rcmotor(unsigned char );
extern unsigned int hcmd_read_rcpneumatic(unsigned char );
extern void hcmd_write_rcstate(unsigned char );
extern unsigned char hcmd_read_rcstate(void );
extern void hcmd_drive_block(unsigned char, signed char, unsigned char);

#endif

