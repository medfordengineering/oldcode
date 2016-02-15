/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue Sep 15 15:52:29 EDT 2009
-----------------------------------------------------------------
============================================================== */

#ifndef _TWIM_XBEEMOD_H_
#define _TWIM_XBEEMOD_H_

#ifndef _SLAVE
	#include <twi_master.h>
#endif
#include <string.h>
#include <delay.h>

#define MAX_MSG_LENGTH					12

#define CMD_XBEE_WRITE_TEXT				0x25
#define CMD_XBEE_WRITE_CONFIGURATION	0x24
#define CMD_XBEE_WRITE_INTEGER			0x28
#define CMD_XBEE_WRITE_CHARACTER		0x29

#define CMD_XBEE_READ_TEXT				0x26
#define CMD_XBEE_READ_CONFIGURATION		0x23
#define CMD_XBEE_READ_CHARACTER			0x2C
#define CMD_XBEE_READ_INTEGER			0x2D
#define CMD_XBEE_READ_JOY				0x2A

#define CMD_XBEE_CHECK_MESSAGE			0x27
#define CMD_XBEE_DEFAULT				0x2B

#define CMD_XBEE_WRITE_DECIMAL			0x30

#define XBEE_START_BYTE					0xFE

#define CMD_SERVO_INIT					0x80
#define CMD_SERVO_DRIVE					0x81

/* LCD module functions */
extern void xbeemod_send_character(char);
extern void xbeemod_send_text(char *);
extern void xbeemod_send_integer(signed int );
extern char xbeemod_receive_character(void);
extern void xbeemod_receive_text(signed char *);
extern void xbeemod_receive_joystick(signed char *);
extern char xbeemod_message_waiting(void);
extern void xbeemod_configure(char *);
extern void xbeemod_read_configuration(char *, char *);  
extern void xbeemod_write_configuration(char *, char *);
extern void xbeemod_send_decimal(unsigned int, unsigned char);

extern void xbeemod_servoinit(char);
extern void xbeemod_servodrive(char, int);

#endif
