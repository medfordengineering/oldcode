/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Fri Sep 25 12:52:41 EDT 2009
-----------------------------------------------------------------
============================================================== */

#ifndef _SENSORMOD_H_
#define _SENSORMOD_H_

#include <twi_master.h>
#include <delay.h>
#include <usart.h>

/* Data values */

#define GPS_SECONDS						4
#define GPS_MINUTES						2
#define GPS_HOURS						0
#define GPS_DAY							24	
#define GPS_MONTH						26
#define GPS_YEAR						28
#define GPS_SATELLITES  				16	
#define GPS_FIX							14	
#define GPS_LATHIGH						6	
#define GPS_LATLOW						8
#define GPS_LONHIGH						10
#define GPS_LONLOW						12
#define GPS_ALTITUDE					18
#define GPS_DIRECTION					22
#define GPS_VELOCITY					20
#define SOUND							0	
#define XAXIS							1		
#define YAXIS							2
#define ZAXIS							3
#define TEMPERATURE						4
#define LIGHT							5

#define ALL_DATA						0

//extern unsigned int sensormod_data(char);
extern signed int sensormod_data(char);
extern unsigned int sensormod_gps(char);
extern void compass_write(char);
extern unsigned int sensormod_compass(char);
extern void sensormod_initialize_gps(char);
extern void sensormod_logging(char);
extern void sensormod_upload_range(int, int);
extern void get_dataset(signed int*, unsigned int , unsigned int );
extern void duck_upload(signed int *);
extern void sensormod_set_usart(unsigned int);
extern void sensormod_gpsinit(void);
#endif
