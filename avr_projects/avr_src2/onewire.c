/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   10 January 2006
-----------------------------------------------------------------
============================================================== */

/* NOTES
a) Onewire_read may require additional delays when using clocks > 4mHz
*/

/* ONEWIRE ROUTINES */

#include <onewire.h>

/* initialize any onewire device */
//signed char onewire_reset(void)
signed char onewire_reset(char ow_pin)
{
	char value = 0;
 	
	clear_bit(OW_PRT_DQ, ow_pin);	// Pull DQ low to start pulse	
	output_bit(OW_DIR_DQ, ow_pin);	
	_delay_us(500);

	input_bit(OW_DIR_DQ, ow_pin);	// Allow DQ to float high
	_delay_us(75);

	if (test_bit(OW_VAL_DQ, ow_pin))
		value = -1;						// Error: No device found

	_delay_us(500);
	return value;
}

/* write byte to a onewire device */
void onewire_writebyte(char ow_pin, char command)
{
 	char x;
 	
 	for (x = 0; x < 8; x++)
	{
		clear_bit(OW_PRT_DQ, ow_pin);	// Pull DQ low for 60us then let float high
		output_bit(OW_DIR_DQ, ow_pin);

		if ((command & 0x01) == 0){				// Write zero time slot
			_delay_us(60);
			input_bit(OW_DIR_DQ, ow_pin);	
		}
		else{									// Write one time slot
			_delay_us(5);
			input_bit(OW_DIR_DQ, ow_pin);	
			_delay_us(60);

		}
		command >>= 1;
 	}
}

//unsigned char onewire_readbyte(void)
unsigned char onewire_readbyte(char ow_pin)
{
 	char x;
 	char value;

 	for(x = 0; x < 8; x++)
 	{
		value >>= 1;      					// Shift down one bit 
		value |= 0x80;						// Set MSB
		clear_bit(OW_PRT_DQ, ow_pin);	// Pull DQ low and test for 1 or 0
		output_bit(OW_DIR_DQ, ow_pin);			
		_delay_us(5);
		input_bit(OW_DIR_DQ, ow_pin);	
		_delay_us(5);

		if((test_bit(OW_VAL_DQ, ow_pin)) == 0)
			value &= 0x7F;					// Set MSB to zero if no pulse
		_delay_us(60);
 	}
 return value;
}
