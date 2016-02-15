/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2007.  All rights reserved
 
 Version Date: Mon Nov 12 12:17:50 EST 2007
-----------------------------------------------------------------
============================================================== */

#include "shift.h"

void shift595_init( void )
{
	/* Initialize data and clock pins */
	
	/* Enable register output pin */
	//output_bit(SHIFT_REG_DIR, SHIFT_REG_PIN);
	set_bit(SHIFT_REG_DIR, SHIFT_REG_PIN);

	/* Initialize register to zero */	
	clear_bit(SHIFT_REG_PRT, SHIFT_REG_PIN);

}

void shift595_out( char data )
{
	/* Send data through standard shift out */
	shift_out(data);	

	/* Set data into 595 by toggling the register pin */
	set_bit(SHIFT_REG_PRT, SHIFT_REG_PIN);
	clear_bit(SHIFT_REG_PRT, SHIFT_REG_PIN);

}
