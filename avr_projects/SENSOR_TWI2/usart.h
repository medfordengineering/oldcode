/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2010.  All rights reserved

Version Date: Mon Jun 28 13:51:04 EDT 2010
-----------------------------------------------------------------
============================================================== */
#ifndef _USART_H_
#define _USART_H_

#include "machinescience.h"
#include <avr/pgmspace.h> 

/* USART DEFINES */
#define CARRIAGE_RETURN 		0x0D
#define LINE_FEED				0x0A
#define BELL 					0x07

extern void usart_init ( unsigned long );
extern unsigned char usart_read(void);
extern void usart_write (unsigned char);
extern void usart_text ( const char * );
#endif
