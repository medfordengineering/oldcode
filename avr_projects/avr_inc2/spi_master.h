/* -------------------------------------------------------------
 Machine Science API
 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue May  5 16:29:15 EDT 2009
 
-----------------------------------------------------------------
============================================================== */
#ifndef _SPI_MASTER_H_
#define _SPI_MASTER_H_

#define SPI_DIR					DDRB
#define SPI_PRT					PORTB

#define CS_PIN					2
#define MOSI_PIN				3
#define MISO_PIN				4
#define SCK_PIN					5

/*
#define spi_start_command()		clear_bit(SPI_PRT, CS_PIN);
#define spi_end_command() 		set_bit(SPI_PRT, CS_PIN);
*/

#define spi_start_command()		clear_bit(PORTD, CS_PIN);
#define spi_end_command() 		set_bit(PORTD, CS_PIN);

extern void spi_master_init(void);
extern unsigned char spi_transfer(char);
 
#endif
