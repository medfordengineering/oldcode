/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2007.  All rights reserved

 Version Date: Mon Nov 19 08:54:59 EST 2007
-----------------------------------------------------------------
============================================================== */
#ifndef _SHIFT_OUT_
#define _SHIFT_OUT_

//#include "machinescience.h"
#include <ms.h>

#define SHIFT_DATA_PRT	PORTC
#define SHIFT_DATA_DIR	DDRC
#define SHIFT_DATA_PIN	2

#define SHIFT_CLOCK_PRT PORTC
#define SHIFT_CLOCK_DIR	DDRC
#define SHIFT_CLOCK_PIN	3

#define SHIFT_REG_PRT	PORTD
#define SHIFT_REG_DIR	DDRD
#define SHIFT_REG_PIN	5

#define led_display( c )	shift595_out( c )
#define led_init( c )		shift595_init( c )

extern void shift_init( void );
extern void shift_out( char );
extern void shift595_init( void );
extern void shift595_out( char );
extern void bar_graph( char );
#endif

