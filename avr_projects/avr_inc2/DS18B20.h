/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2004.  All rights reserved

 Version Date:   12 January 2005
-----------------------------------------------------------------
============================================================== */
#ifndef _DS18B20_H_
#define _DS18B20_H_

# include "machinescience.h"

/* basic defines for the DS18B20 */
#define TEMPERATURE   		0x00
#define SIGN    			0x01
#define FRACTION   			0x02
#define TEMPERATURE_12BIT	0x03
#define WHOLE_INTEGER		0x00

#define SKIP_ROM   			0xCC
#define SEARCH_ROM   		0xF0
#define READ_ROM   			0x33
#define MATCH_ROM   		0x55

#define ALARM_SEARCH  		0xEC
#define CONVERT_TEMP  		0x44
#define READ_SCRATCHPAD  	0xBE

/* DS18B20 ROUTINES */
extern unsigned int temp_sensor_read(char, char);
extern signed int convert_2scomplement(unsigned int, char);

#endif
