/* -------------------------------------------------------------
 Machine Science API
 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   13 June 2008
 
 Test
-----------------------------------------------------------------
============================================================== */

#ifndef _PINS_H_
#define _PINS_H_

//#include "machinescience.h"
#include <avr/pgmspace.h>

#define PORT_B0 0
#define PORT_B1 1
#define PORT_B2 2
#define PORT_B3 3
#define PORT_B4 4
#define PORT_B5 5
#define PORT_B6 6
#define PORT_B7 7
#define PORT_C0 8
#define PORT_C1 9
#define PORT_C2 10
#define PORT_C3 11
#define PORT_C4 12
#define PORT_C5 13
#define PORT_C6 14
#define PORT_C7 15
#define PORT_D0 16
#define PORT_D1 17
#define PORT_D2 18
#define PORT_D3 19
#define PORT_D4 20
#define PORT_D5 21
#define PORT_D6 22
#define PORT_D7 23

#define portModeRegister(P) ( (volatile unsigned char *)( pgm_read_byte( port_to_mode_PGM + (P))) );

extern const unsigned char PROGMEM port_to_mode_PGM[]; 
extern void output_pin(unsigned char );
extern void input_pin(unsigned char pinRef);
extern void high_pin(unsigned char pinRef);
extern void low_pin(unsigned char pinRef);
extern void toggle_pin(unsigned char pinRef);
extern char pin_value(unsigned char pinRef);
extern void pullup_on(unsigned char pinRef);
extern void pullup_off(unsigned char pinRef);

#endif
