/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue Jun 15 10:19:22 EDT 2010
-----------------------------------------------------------------
============================================================== */

#ifndef _GPS_H
#define _GPS_H_

#include <ms.h>
#include <avr/interrupt.h>
#include <usart.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h> 

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

/* MTK GPS value */
#define GPS_BAUD 57600

/* MTK disable message */
#define GPS_DIS_GGA_MSG PSTR("$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28")
#define GPS_DIS_GLL_MSG PSTR("$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28")
#define GPS_DIS_GSA_MSG PSTR("$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28")
#define GPS_DIS_GSV_MSG PSTR("$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28")
#define GPS_DIS_RMC_MSG PSTR("$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28")
#define GPS_DIS_VTG_MSG PSTR("$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28")

/* MTK querry message */
#define GPS_QRY_GGA_MSG PSTR("$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29")
#define GPS_QRY_RMC_MSG PSTR("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29")

/* SiRF baudrate message */
#define GPS_COMM_9600  PSTR("$PSRF100,1,9600,8,1,0*0D")
#define GPS_COMM_4800  PSTR("$PSRF100,1,4800,8,1,0*0E")
#define GPS_COMM_19200  PSTR("$PSRF100,1,19200,8,1,0*38")

#define GPS_END_MSG PSTR("\r\n")

extern volatile unsigned char gpsConvertState;
extern unsigned char parsedData[];

extern void gps_init(void);
extern signed int gps_data(int);

#endif
