/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue Jun 15 10:19:22 EDT 2010
-----------------------------------------------------------------
============================================================== */

#ifndef _GPS_SENSOR_H_
#define _GPS_SENSOR_H_

#include "machinescience.h"

/* SiRF disable message */
#define GPS_DIS_GGA_MSG PSTR("$PSRF103,00,00,00,01*24")
#define GPS_DIS_GLL_MSG PSTR("$PSRF103,01,00,00,01*25")
#define GPS_DIS_GSA_MSG PSTR("$PSRF103,02,00,00,01*26")
#define GPS_DIS_GSV_MSG PSTR("$PSRF103,03,00,00,01*27")
#define GPS_DIS_RMC_MSG PSTR("$PSRF103,04,00,00,01*20")
#define GPS_DIS_VTG_MSG PSTR("$PSRF103,05,00,00,01*21")

/* SiRF querry message */
#define GPS_QRY_GGA_MSG PSTR("$PSRF103,00,01,00,01*25")
#define GPS_QRY_RMC_MSG PSTR("$PSRF103,04,01,00,01*21")

/* SiRF baudrate message */
#define GPS_COMM_9600  PSTR("$PSRF100,1,9600,8,1,0*0D")
#define GPS_COMM_4800  PSTR("$PSRF100,1,4800,8,1,0*0E")
#define GPS_COMM_19200  PSTR("$PSRF100,1,19200,8,1,0*38")

#define GPS_END_MSG PSTR("\r\n")

/* Sensor Module Convert States */
/*
#define NO_STATE				0
#define CAPTURE_GGA				1
#define WAIT_GGA				2 
#define PARSE_GGA				3
#define CAPTURE_RMC				4
#define WAIT_RMC				5
#define PARSE_RMC				6
#define CAPTURE_ADC				7	
#define FLASH_RECORD			8
*/

extern volatile unsigned char gpsConvertState;
extern unsigned char parsedData[];

extern void gps_init(void);
extern void gps_parse_gga(void);
extern void gps_parse_rmc(void);

#endif
