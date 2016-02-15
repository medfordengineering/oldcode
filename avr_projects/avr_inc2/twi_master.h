/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue Mar 31 13:13:03 EDT 2009
-----------------------------------------------------------------
============================================================== */
#ifndef _TWI_MASTER_H_
#define _TWI_MASTER_H_

//#include <machinescience.h>
#include <ms.h>
#include <avr/interrupt.h>

#define TWI_START		0x08
#define TWI_RESTART		0x10
#define SLAW_ADR_ACK	0x18
#define SLAW_ADR_NACK	0x20
#define SLAW_DAT_ACK	0x28
#define SLAW_DAT_NACK	0x30
#define SLAR_ADR_ACK	0x40
#define SLAR_ADR_NACK	0x48
#define SLAR_DAT_ACK	0x50
#define SLAR_DAT_NACK	0x58
#define TWI_BUS_ERROR	0x38

#define TWI_WRITE		0
#define TWI_READ		1

/* In passive mode the TWI is enabled but the TWI interrupts are disabled. This  means it will not acknowledge
 * requests and is defined as not busy */
#define TWIMASTERPASSIVE  	(1<<TWEN)|(0<<TWIE)|(0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)

/* In Active mode the TWI will acknowledge request and is defined as busy. This is the same as twiMasterNACK.
 * It is defined separately because in Slave receiver mode the ACK bit is not relavant, and makes more sense
 * as a function name for receiver mode than NACK. */
#define TWIMASTERACTIVE		(1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)

/* In Slave transmitter mode, this will expect a NACK from the Master after the next byte and last is sent.
 * Not sure it matters what we expect.  It is the same as Active except for the name. */
#define TWIMASTERNACK 		(1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)                       

/* In Slave transmitter mode, this will expect an ACK from the Master after each byte is sent. */
#define TWIMASTERACK 		(1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)                        

/* Initiate a START condition */
#define TWIMASTERSTART 		(1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|(0<<TWWC)                     

/* Initiate a STOP condition */
#define TWIMASTERSTOP 		(1<<TWEN)|(0<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC)                      

#define TWI_MAX_BUF		300

/* Addresses for all the modules */
#define GENERAL_CALL			0x00
//#define RESERVED				0x01
#define LCD_MODULE				0x02
#define KEYPAD_MODULE			0x03
#define MOTOR_MODULE			0x04	
#define MOTOR_MODULEA			0x05	
#define MOTOR_MODULEB			0x06	
#define MOTOR_MODULEC			0x07	
#define PINPOINT_MODULE			0x08
#define MP3_MODULE				0x09
#define XBEE_MODULE				0x0A
#define SENSOR_MODULE			0x0B
#define PS2XIP_MODULE			0x12
#define COMPASS_MODULE			0x21

extern volatile unsigned char twiData[TWI_MAX_BUF];
extern volatile unsigned int twiMsgSize;

extern void network_control(unsigned char);
extern inline unsigned char twiBusy(void);
extern void twiMasterInit (unsigned long);

#endif
