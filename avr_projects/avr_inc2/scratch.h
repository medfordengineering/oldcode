/* --------------------------------------------------------------
   Machine Science API

   (c) Copyright Machine Science, 2008.  All rights reserved

   Version Date	  16 June 2008
-----------------------------------------------------------------
==============================================================+*/

#ifndef _SCRATCH_H_
#define _SCRATCH_H_

//#include "machinescience.h"
#include <ms.h>
#include "adc.h"
#include "usart.h"

#define RESISTANCE_D 0
#define RESISTANCE_C 1
#define RESISTANCE_B 2
#define BUTTON 3
#define RESISTANCE_A 4
#define LIGHT 5
#define SOUND 6
#define SLIDER 7

#define FIRMWARE_ID 0x04

extern int value_array[];

extern void send_sensor( int* );
extern void scratch_write( char, int );
extern void scratch_send( char, int );

#endif
