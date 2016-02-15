/* -------------------------------------------------------------
Machine Science API
 
(c) Copyright Machine Science, 2012.  All rights reserved

Version Date: Mon Oct 25 11:03:19 EDT 2012
-----------------------------------------------------------------
============================================================== */
#ifndef _MS_PINS_H_
#define _MS_PINS_H_

//#include <machinescience.h>
#include <mxapi.h>
#include <avr/pgmspace.h>

#define portPin(P) ( pgm_read_byte( portPinPGM + (P) ) )
#define maskPin(P) ( pgm_read_byte( maskPinPGM + (P) ) )
#define portRegister(P) ( (volatile unsigned char *)( pgm_read_word( portRegisterPGM + (P))) )
#define outputPort(P) ( (volatile unsigned char *)( pgm_read_word( outputPortPGM + (P))) )
#define inputPort(P) ( (volatile unsigned char *)( pgm_read_word( inputPortPGM + (P))) )

#define PRTD 4
#define PRTB 2
#define PRTC 3

#define PORT_D0 0
#define PORT_D1 1
#define PORT_D2 2
#define PORT_D3 3
#define PORT_D4 4
#define PORT_D5 5
#define PORT_D6 6
#define PORT_D7 7
#define PORT_B0 8
#define PORT_B1 9
#define PORT_B2 10
#define PORT_B3 11
#define PORT_B4 12
#define PORT_B5 13

//THIS WORKS WITH BREADBOARD
#define PORT_C0 8
#define PORT_C1 9
#define PORT_C2 10
#define PORT_C3 11
#define PORT_C4 12
#define PORT_C5 13
#define PORT_C6 14
#define PORT_C7 15

#define NOT_A_PIN 0
#define NOT_A_PORT 0

extern void output_pin(unsigned char );
extern void input_pin(unsigned char );
extern void high_pin(unsigned char );
extern void low_pin(unsigned char );
extern int pin_value(unsigned char );
extern void pullup_on(unsigned char pin);
extern void pullup_off(unsigned char pin);

const unsigned int PROGMEM portRegisterPGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(unsigned int) &DDRB,
	(unsigned int) &DDRC,
	(unsigned int) &DDRD,
};

const unsigned char PROGMEM portPinPGM[] = {
	PRTD, /* 0 */
	PRTD,
	PRTD,
	PRTD,
	PRTD,
	PRTD,
	PRTD,
	PRTD,
	PRTB, /* 8 */
	PRTB,
	PRTB,
	PRTB,
	PRTB,
	PRTB,
	PRTC, /* 14 */
	PRTC,
	PRTC,
	PRTC,
	PRTC,
	PRTC,
};

const unsigned char PROGMEM maskPinPGM[] = {
	_BV(0), /* 0, port D */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(0), /* 8, port B */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(0), /* 14, port C */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
};

const unsigned int PROGMEM outputPortPGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(unsigned int) &PORTB,
	(unsigned int) &PORTC,
	(unsigned int) &PORTD,
};

const unsigned int PROGMEM inputPortPGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(unsigned int) &PINB,
	(unsigned int) &PINC,
	(unsigned int) &PIND,
};

#endif
