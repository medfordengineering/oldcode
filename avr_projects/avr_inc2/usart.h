/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2004.  All rights reserved

Version Date: Mon Nov 12 11:18:49 EST 2007
-----------------------------------------------------------------
============================================================== */
#ifndef _USART_H_
#define _USART_H_

#include <ms.h>
#include <ms_print.h>
#include <avr/pgmspace.h> 

/* USART DEFINES */
#define CARRIAGE_RETURN 		0x0D
#define LINE_FEED				0x0A
#define BELL 					0x07

/* SPECIAL FOR BUFFER */
#define RX_BUFFER_SIZE 25
//unsigned char rxBuffer[RX_BUFFER_SIZE+1];
signed char rxBuffer[RX_BUFFER_SIZE+1];
int rxWriteIndex;
int rxReadIndex;
volatile int rxCounter;
char rxBufferOverflow;

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
extern void usart_const_text(const char *);
//extern void usart_decimal(int);
extern void usart_decimal(unsigned int, char);
extern void usart_binary(unsigned int, char);
extern void usart_hexidecimal(unsigned int, char);
extern void usart_signed(signed int, char);
extern void usart_digits(signed int, char, char, char);
extern char usart_buffer_read(void);
extern void usart_interrupt_rx(unsigned char);
extern void usart_interrupt_tx(unsigned char);
extern void usart_return(void);
#endif
