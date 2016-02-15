/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue Jun 15 10:19:22 EDT 2010
-----------------------------------------------------------------
============================================================== */

#ifndef _GPS_H
#define _GPS_H_

//#include <machinescience.h>
#include <mxapi.h>
//#include <depco.h>


#include <avr/interrupt.h>
#include <ms_usart.h>
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

/* SiRF disable message */
#define GPS_DIS_GGA_MSG PSTR("$PSRF103,00,00,00,01*24")
#define GPS_DIS_GLL_MSG PSTR("$PSRF103,01,00,00,01*25")
#define GPS_DIS_GSA_MSG PSTR("$PSRF103,02,00,00,01*26")
#define GPS_DIS_GSV_MSG PSTR("$PSRF103,03,00,00,01*27")
#define GPS_DIS_RMC_MSG PSTR("$PSRF103,04,00,00,01*20")
#define GPS_DIS_VTG_MSG PSTR("$PSRF103,05,00,00,01*21")

/* MTK disable message */
#define GPS_DIS_MSG PSTR("$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28")

/* SiRF querry message */
#define GPS_QRY_GGA_MSG PSTR("$PSRF103,00,01,00,01*25")
#define GPS_QRY_RMC_MSG PSTR("$PSRF103,04,01,00,01*21")

/* MTK querry message */
#define MTK_QRY_GGA_MSG PSTR("$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0*29")
#define MTK_QRY_RMC_MSG PSTR("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29")

/* SiRF baudrate message */
#define GPS_COMM_9600  PSTR("$PSRF100,1,9600,8,1,0*0D")
#define GPS_COMM_4800  PSTR("$PSRF100,1,4800,8,1,0*0E")
#define GPS_COMM_19200  PSTR("$PSRF100,1,19200,8,1,0*38")

#define GPS_END_MSG PSTR("\r\n")

#define PARSED_BUFFER_SIZE			90

/* Map legacy functions to SIRF functions */
#define gps_init 	sirf_init
#define gps_data 	sirf_data

extern volatile unsigned char gpsConvertState;
extern volatile unsigned char gpsCaptureState;

extern unsigned char parsedData[];
extern unsigned char gpsParsed[];

extern void mtk_init(void);
extern signed int mtk_data(int);

extern void sirf_init(void);
extern signed int sirf_data(int);

extern void gps_parse_gga(void);
extern void gps_parse_rmc(void);
extern void gps_send_msg(const char *);

#endif

