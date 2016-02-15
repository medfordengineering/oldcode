/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   10 January 2006
-----------------------------------------------------------------
============================================================== */

/*NOTES:
*/

/* DS18B20 ROUTINES */
#include "DS18B20.h"
#include "onewire.h"

/* one function for reading from a DS18B20 */
unsigned int temp_sensor_read(char ow_pin, char command)
{
	unsigned char low_byte;
	unsigned char high_byte;
	unsigned int value;

	onewire_reset(ow_pin);								/* send convert temperature command */
	onewire_writebyte(ow_pin, SKIP_ROM);
	onewire_writebyte(ow_pin, CONVERT_TEMP);

	onewire_reset(ow_pin);						/* skip the rom read for a single device */
	onewire_writebyte(ow_pin, SKIP_ROM);
	onewire_writebyte(ow_pin, READ_SCRATCHPAD);
	low_byte = onewire_readbyte(ow_pin);
	high_byte = onewire_readbyte(ow_pin);
	value = ((unsigned int) high_byte << 8) + low_byte;
	value = convert_2scomplement(value, command);
	return value;
}

signed int convert_2scomplement(unsigned int value, char command)
{
    switch (command)
    {
        case WHOLE_INTEGER:	
			if (value & 0x8000){
				value = (~value + 1) >> 4;				
				value *= -1;
			}
			else
				value >>= 4;			
        	break;

        case FRACTION:	
			if (value & 0x8000)
				value = (~value + 1); 
			value = (value & 0x000F) * 625;			
        	break;
    }
	return (signed int) value;
}
