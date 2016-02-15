/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2004.  All rights reserved

Version Date: Mon Nov 12 11:18:49 EST 2007
-----------------------------------------------------------------
============================================================== */
#ifndef _MS_USART_H_
#define _MS_USART_H_

#include <machinescience.h>
#include <ms_print.h>
#include <avr/pgmspace.h> 

/* USART DEFINES */
#define CARRIAGE_RETURN 		0x0D
#define LINE_FEED				0x0A
#define BELL 					0x07

/* basic usart defines */
//extern void usart_init(unsigned long);
extern void usart_init(void);
extern void usart_baud(unsigned long);
extern void usart_size(unsigned char);
extern void usart_rx(unsigned char);
extern void usart_tx(unsigned char);
extern void usart_write(unsigned char);
extern unsigned char usart_read(void);
extern void usart_text(char *);
extern void usart_textln(char *);
extern void usart_const_text(const char *);
extern void usart_decimal(unsigned int, char);
extern void usart_binary(unsigned int, char);
extern void usart_hexidecimal(unsigned int, char);
extern void usart_signed(signed int, char);
extern void usart_digits(signed int, char, char, char);
extern void usart_return(void);
extern void ms_sprintf(signed int, char, char, char, char *);

#endif
