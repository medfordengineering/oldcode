/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2007.  All rights reserved
 
 Version Date: Mon Nov 12 12:17:50 EST 2007
-----------------------------------------------------------------
============================================================== */

#include "shift.h"

void shift_init( void )
{
	/* Enable data output pin */
	//output_bit(SHIFT_DATA_DIR, SHIFT_DATA_PIN);
	set_bit(SHIFT_DATA_DIR, SHIFT_DATA_PIN);
	
	/* Enable clock output pin */
	//output_bit(SHIFT_CLOCK_DIR, SHIFT_CLOCK_PIN);
	set_bit(SHIFT_CLOCK_DIR, SHIFT_CLOCK_PIN);

	/* Initialize clock to zero */	
	clear_bit(SHIFT_CLOCK_PRT, SHIFT_CLOCK_PIN);
	
}

void shift_out( char data )
{
	char i;
	for (i = 0; i < 8; i++){

		/* shift in MSB to LSB */
		if ( data & 0x80 ) 
			set_bit(SHIFT_DATA_PRT, SHIFT_DATA_PIN);
		else
			clear_bit(SHIFT_DATA_PRT, SHIFT_DATA_PIN);

		/* clock data into register */
		set_bit(SHIFT_CLOCK_PRT, SHIFT_CLOCK_PIN);
		clear_bit(SHIFT_CLOCK_PRT, SHIFT_CLOCK_PIN);

		data <<= 1;
	}
}


