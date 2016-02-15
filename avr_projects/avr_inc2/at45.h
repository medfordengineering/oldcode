/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2010.  All rights reserved

Version Date: Mon Jun  7 12:48:05 EDT 2010
-----------------------------------------------------------------
============================================================== */

#ifndef _AT45_H
#define _AT45_H

/* Defines for modes 0 or 3 */
/* NOTE: OTHER MODES MUST NOT BE USED */
#define AT45_STATUS					0xD7
#define AT45_ID						0x9F
#define AT45_WRITE_TO_BUFFER_1   	0x84
#define AT45_WRITE_TO_BUFFER_2   	0x87
#define AT45_READ_FROM_BUFFER_1		0xD4	
#define AT45_READ_FROM_BUFFER_2		0xD6	

#define AT45_WRITE_FROM_BUFFER_1 	0x83
#define AT45_WRITE_FROM_BUFFER_2 	0x86
#define AT45_READ_FROM_PAGE      	0xD2
#define AT45_BLOCK_ERASE			0x50
#define AT45_PAGE_ERASE				0x81

#define STATUS_READY_BIT 			0x80

//typedef enum {OK = 1, ERROR = 0} status;

extern unsigned char at45_status(void);
extern status at45_buffer_write(unsigned int , unsigned char *, unsigned int );
extern status at45_buffer_read(unsigned int, unsigned char *, unsigned int);
extern status at45_page_write(unsigned int );
extern status at45_page_read(unsigned int , unsigned int , volatile unsigned char *, unsigned int );
extern status at45_chip_erase(void);
extern status at45_block_erase(unsigned int);
#endif 
