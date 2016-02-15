/* -------------------------------------------------------------
 Machine Science API
 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue May  5 16:29:15 EDT 2009
 
-----------------------------------------------------------------
============================================================== */
#ifndef _TWIM_LCMD_H_
#define _TWIM_LCMD_H_

#include <twi_masterslave.h>
#include <stdlib.h>

#define CMD_LC_DRIVEMOTOR		0x37
#define CMD_LC_SOLENOID 		0x38
#define CMD_LC_DRIVEROBOT		0x39
#define CMD_LC_CURRENT			0x3A
#define CMD_LC_FREQUENCY		0x3B
#define CMD_LC_SETLIMIT			0x3C
#define CMD_LC_COUNT			0x3D
#define CMD_LC_LIMITINIT		0x3E
#define CMD_LC_ENCODERINIT		0x3F
#define CMD_LC_WDTINIT			0x40
#define CMD_LC_FEEDBACKINIT		0x41
#define CMD_LC_FULLDRIVE		0x42

#define MOTORA					0	
#define MOTORB					1	

extern void motor_drive(unsigned char, signed char);
extern void drive_solenoid(unsigned char);
extern unsigned char read_current(unsigned char);
extern void drive_robot(unsigned char, unsigned char);
extern unsigned int read_counter(unsigned char);
extern void drive_full(char , signed int );

#endif

