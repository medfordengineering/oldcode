/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Mon Oct 18 14:43:08 EDT 2010
-----------------------------------------------------------------
============================================================== */

#ifndef _SERVO_H_
#define _SERVO_H_

#define OUTPUT		0
#define INPUT		1
#define PNEUMATIC	2
#define SERVO		3

/* Full on is the multiplier used to get a 2000ms (.002seconds) delay. It equals
 * (.002 * FOSC) / prescale.  Full off equals (.001 * FOSC) / prescale. */

/* The servo delay equals the length of each of the 8 pulses plus the length
 * of the defined servo pulse delay. For example if the defined servo pulse delay is
 * set for neutral and all the servos are set for neutral than the total time
 * between servo pulses would be 8 * .0015. Note that the total time between pulses
 * varies based on the length of pulses defined by the user. */

/* For 12mhz clocks and greater */
#define PRESCALE_SERVO			256
#define SERVO_PRESCALE			6


#define SERVO_FULL_ON			(.0020 * FOSC) / PRESCALE_SERVO
#define SERVO_FULL_OFF			(.0010 * FOSC) / PRESCALE_SERVO
#define MOTOR_FULL_ON			(.0017 * FOSC) / PRESCALE_SERVO
#define MOTOR_FULL_OFF			(.0013 * FOSC) / PRESCALE_SERVO
#define MOTOR_FRACTION 			(MOTOR_FULL_ON - MOTOR_FULL_OFF) / 2
#define GEARS_FRACTION 			(SERVO_FULL_ON - SERVO_FULL_OFF) / 2
#define SERVO_PULSE_DELAY		0xFF //Maximum possible

#define SERVO_NEUTRAL			SERVO_FULL_OFF + ((SERVO_FULL_ON - SERVO_FULL_OFF) / 2)

#ifndef _TWIM_HCMD_H_
	#define FORWARD 				0x00
	#define REVERSE					0x01
	#define SPIN_RIGHT				0x02
	#define SPIN_LEFT 				0x03
	#define MOTORA					0x00
	#define MOTORB					0x01
	#define MOTORC					0x02
	#define MOTORD					0x03
	#define STOP					0x04
#endif

extern unsigned char servoPortMask;
extern unsigned char masterServoCounter; 
extern unsigned char masterServoControl[9];

extern void servo_port(unsigned char, unsigned char);
extern void servo_init(void);
extern void servo_setup(unsigned char);
extern void servo_control(unsigned char);
extern void servo_position(unsigned char , unsigned int );
extern void servo_robot(char, signed char);
extern void servo_motor(unsigned char, signed char);

#endif
