
/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Mon Oct 18 14:43:08 EDT 2010
-----------------------------------------------------------------
============================================================== */

#ifndef _SERVO_H_
#define _SERVO_H_

#include <timers.h>
#include <pins.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <ms.h>

#define OUTPUT		0
#define INPUT		1
#define PNEUMATIC	2
#define SERVO		3

#define SERVO_FULL_ON		250
#define SERVO_FULL_OFF		125
#define SERVO_RANGE			SERVO_FULL_ON - SERVO_FULL_OFF
#define SERVO_NEUTRAL		SERVO_RANGE / 2
#define SERVO_MULTIPLYER	125
#define SERVO_DIVISOR		100
#define SERVO_DEGREE_DIVISOR 180
#define SERVO_PULSE_DELAY	140

#ifndef _TWIM_HCMD_H_
#define FORWARD 				0x00
#define REVERSE					0x01
#define SPIN_RIGHT				0x02
#define SPIN_LEFT 				0x03
#define MOTORA					0x00
#define MOTORB					0x01
#define STOP					0x04
#endif

extern unsigned char servoPortMask;
extern unsigned char masterServoCounter; 
extern unsigned char masterServoControl[9];

extern void servo_port(unsigned char, unsigned char);
extern void servo_init(void);
extern void servo_control(unsigned char);
extern void servo_position(unsigned char , unsigned char );
extern void servo_robot(char, signed char);
extern void servo_motor(unsigned char, signed char);

#endif
