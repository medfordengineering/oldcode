/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2010.  All rights reserved

Version Date: Thu Aug 25 10:39:15 EDT 2011
-----------------------------------------------------------------
============================================================== */

#include <spi_master.h>

void spi_master_init(void)
{
	unsigned char clr;

	/* Set bits for sensor board */
	set_bit(DDRD, 2);
	set_bit(DDRD, 3);
	set_bit(PORTD, 3);

	/* Set MOSI, SCK and CS output, and MISO as input */
	SPI_DIR = (1<<MOSI_PIN)|(1<<SCK_PIN)|(0<<MISO_PIN)|(1<<CS_PIN);
	
		/* Enable SPI, Master, Mode 0, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(0<<CPOL)|(0<<CPHA);

	/* Enable SPI, Master, Mode 3, set clock rate fck/16 */
	//SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<CPOL)|(1<<CPHA);

	/* Clear status and data registers */
	clr = SPSR;
	clr = SPDR;

	/* Set SPI in idle */
	spi_end_command();
}


unsigned char spi_transfer(char data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF))); 
	return SPDR;
}

