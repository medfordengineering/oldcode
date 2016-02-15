/* -------------------------------------------------------------
Machine Science API
 
(c) Copyright Machine Science, 2012.  All rights reserved

Version Date: Mon Oct 25 11:03:19 EDT 2010
-----------------------------------------------------------------
============================================================== */
#ifndef _MACHINESCIENCE_H_
#define _MACHINESCIENCE_H_

#include <avr/io.h>
#include <ms_pins.h>
#include <ms_analog.h>

#define MS_BREADBOARD
//#define MS_ARDUINO
//#define XIPMOD

#ifdef MS_BREADBOARD
	#define FOSC 8000000L
#endif

#ifdef XIPMOD
	#define FOSC 20000000L
#endif

#ifdef MS_ARDUINO
	#define FOSC 16000000L
#endif

//#define FOSC 16000000L
//#define FOSC 8000000L
//#define FOSM FOSC/1000000L

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
#define NO			0	
#define YES			1
#define PRESSED 	1

/* Universal EEPROM Registers */
#define TWI_ADDRESS		7

#define set_bit(var, pin)		var |= 1<<(unsigned char) pin
#define clear_bit(var, pin)		var &= ~(1<<(unsigned char)pin)
#define test_bit(var, pin)		((var & (1<<(unsigned char) pin)) >> pin)
#define toggle_bit(var, pin)	var ^= 1<<(unsigned char) pin	

#define clr_bit(var, pin) 		clear_bit(var,pin)
#define tog_bit(var, pin)		toggle_bit(var,pin)
#define tst_bit(var, pin)		var & (1<<(unsigned char) pin)

//#define set_bit(var, pin)		var |= 1<<(unsigned char) pin
//#define clr_bit(var, pin)		var &= ~(1<<(unsigned char)pin)
//#define tog_bit(var, pin)		var ^= 1<<(unsigned char) pin	

#define input_bit(var, pin)		var &= ~(1<<(unsigned char) pin)
#define output_bit(var, pin)	var |= 1<<(unsigned char) pin
#define digital_read(pin) 		pin_value(pin + 8)
#define quit()					while(1)

#endif

