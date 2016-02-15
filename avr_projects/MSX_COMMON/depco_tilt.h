/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Fri Sep 25 12:52:41 EDT 2009
-----------------------------------------------------------------
============================================================== */

#ifndef _TILTSENSE_H_
#define _TILTSENSE_H_

#include <twi_masterslave.h>

/* Commands for ultra sensor */

extern void tilt_init(void);
extern void axcell_init(void);
extern void axcell_setdata(unsigned char, unsigned char);
extern void axcell_getdata(char, signed int *);
extern void axcell_init(void);
extern signed int axcell_read(char );
extern void gyro_init(void);
extern void gyro_writebyte(unsigned char , unsigned char );
extern unsigned char gyro_readbyte(char );
extern signed int gyro_read(char );
extern void gyro_readburst(signed int * );

#define CMD_TILT_DEVICEID		0x00
#define CMD_TILT_THRESH_TAP		0x1D 		//Tap threshold
#define CMD_TILT_DATAX0			0x32		//X axis data 0
#define CMD_TILT_POWER_CTL		0x2D		//Power saving features control
#define CMD_TILT_BW_RATE		0x2C		//Data rate and power mode control
#define CMD_TILT_DATA_FORMAT	0x31		//Data format control

#define AX_AXIS					0
#define AY_AXIS					1
#define AZ_AXIS					2

#define GX_AXIS					0
#define GY_AXIS					1
#define GZ_AXIS					2

#endif
