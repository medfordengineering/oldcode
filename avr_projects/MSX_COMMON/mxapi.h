#ifndef _MXAPI_H_
#define _MXAPI_H_

#include <ms_pins.h>
#include <ms_delay.h>
#include <avr/io.h>

//THIS MUST BE SET FOR EACH PLATFORM
//#define MS_BREADBOARD
#define XIPMOD

#ifdef MS_BREADBOARD
	#define FOSC 8000000L
	#define SMALL_BUFFER
	#define MOTORA 		0
	#define MOTORB 		1
	#define STOP		0
	#define FORWARD		1
	#define REVERSE		2
	#define SPIN_RIGHT	3
	#define SPIN_LEFT	4
#endif

#ifdef XIPMOD
	#define FOSC 20000000L
	#define SMALL_BUFFER
	#include <twi_masterslave.h>
#endif

#ifdef MS_ARDUINO
	#define FOSC 16000000L
#endif

#ifdef DEPCO_MASTER
	//#define FOSC 16000000L
	#define SMALL_BUFFER
	#define MOTORA 		0
	#define MOTORB 		1
	#define STOP		0
	#define FORWARD		1
	#define REVERSE		2
	#define SPIN_RIGHT	3
	#define SPIN_LEFT	4
	#include <depco_master.h>			//Aparently the order is important here!
	#include <twi_masterslave.h>
	#include <depco_motor.h>
	#include <depco_line.h>
	#include <depco_ir.h>
	#include <depco_tilt.h>
	#include <depco_ultra.h>
	#include <servo.h>
#endif

#define TWIADDR_EEPROM	7

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

#define set_bit(var, pin)		var |= 1<<(unsigned char) pin
#define clear_bit(var, pin)		var &= ~(1<<(unsigned char)pin)
#define test_bit(var, pin)		((var & (1<<(unsigned char) pin)) >> pin)
#define toggle_bit(var, pin)	var ^= 1<<(unsigned char) pin	

#define clr_bit(var, pin) 		clear_bit(var,pin)
#define tog_bit(var, pin)		toggle_bit(var,pin)
#define tst_bit(var, pin)		var & (1<<(unsigned char) pin)

#define input_bit(var, pin)		var &= ~(1<<(unsigned char) pin)
#define output_bit(var, pin)	var |= 1<<(unsigned char) pin
#define digital_read(pin) 		pin_value(pin + 8)
#define quit()					while(1)

#endif


