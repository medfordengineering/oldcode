/* -------------------------------------------------------------
 Machine Science API
 
 (c) Copyright Machine Science, 2007.  All rights reserved

 Version Date: Fri Oct 30 09:52:45 EDT 2009
-----------------------------------------------------------------
============================================================== */
#ifndef _MACHINESCIENCE_H_
#define _MACHINESCIENCE_H_

#define FOSM FOSC/1000000L

/* generic */
#define HIGH   		1
#define LOW       	0
#define TRUE   		1
#define FALSE   	0
#define ON    		1
#define OFF   		0
#define TOGGLE		2
#define ENABLE		1
#define DISABLE		0
#define ENABLED		1
#define DISABLED	0
#define PRESSED		1
#define UNPRESSED	0
#define NO			0	
#define YES			1

#define set_bit(var, pin)		var |= 1<<(unsigned char) pin
#define clear_bit(var, pin)		var &= ~(1<<(unsigned char)pin)
#define test_bit(var, pin)		((var & (1<<(unsigned char) pin)) >> pin)
#define toggle_bit(var, pin)	var ^= 1<<(unsigned char) pin	

//#define input_bit(var, pin)		var &= ~(1<<(unsigned char) pin)
//#define output_bit(var, pin)	var |= 1<<(unsigned char) pin

//#define output_nib(var, pin)    var |= (0x0F << (unsigned char) pin)
//#define input_nib(var, pin)     var &= (0xF0 >> (unsigned char) pin)

#define nop() 					asm volatile("nop\n")
#define end()					while(TRUE)

//#define uint8		uint8_t
//#define sint8		int8_t
//#define uint16		uint16_t
//#define sint16		int16_t

typedef enum {OK = 1, ERROR = 0} status;

#include <avr/io.h>
#include <avr/interrupt.h>

#endif

