/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue Mar 31 13:13:03 EDT 2009
-----------------------------------------------------------------
============================================================== */
#ifndef _TWI_MASTERSLAVE_H_
#define _TWI_MASTERSLAVE_H_

//#include <machinescience.h>
#include <mxapi.h>
//#include <depco.h>

#include <avr/interrupt.h>

/*Master States*/
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
#define SLAW_ARB_LOST	0x38

/*Slave States*/
#define SLAW_ADDR_ACK	0x60
#define SLAW_DATA_ACK   0x80
#define SLAW_STOP		0xA0
#define SLAR_ADDR_ACK  	0xA8
#define SLAR_DATA_ACK	0xB8
#define SLAR_LB_NACK	0xC0
#define SLAR_LB_ACK		0xC8
#define SLAR_ARB_LOST	0x68

/*Miscellaneous States*/
#define ILLEGAL_ST		0x00 

#define TWI_WRITE		0
#define TWI_READ		1
#define TWI_MASTER		0
#define TWI_SLAVE		1
#define TWI_NOSTATE		2

/* NOTE the ATtiny48 does not have enough memory for the larger buffer */
#if defined LARGE_BUFFER
	#define TWI_MAX_BUF		300
#elif defined SMALL_BUFFER
	#define TWI_MAX_BUF		20
#endif

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

#define TWISLAVEACTIVEACK	(1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)                        
#define TWISLAVEACTIVENACK	(1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)                    
#define TWISLAVEPASSIVE		(1<<TWEN)|(0<<TWIE)|(0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)                      
#define TWISLAVEDISABLE		(0<<TWEN)

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
#define LCMOTOR_MODULE			0x0C
#define PS2_MODULE				0x0D
#define PS2XIP_MODULE			0x12

#define IRD_MOD_0				0x13
#define IRD_MOD_1				0x14
#define IRD_MOD_2				0x15
#define IRD_MOD_3				0x16
#define IRD_MOD_4				0x17
#define IRD_MOD_5				0x18
#define IRD_MOD_6				0x19
#define IRD_MOD_7				0x1A

#define LINE_MOD_0				0x2B
#define LINE_MOD_1				0x2C
#define LINE_MOD_2				0x2D
#define LINE_MOD_3				0x2E
#define LINE_MOD_4				0x2F
#define LINE_MOD_5				0x30
#define LINE_MOD_6				0x31
#define LINE_MOD_7				0x32

#define ULTRA_MOD_0				0x23
#define ULTRA_MOD_1				0x24
#define ULTRA_MOD_2				0x25
#define ULTRA_MOD_3				0x26
#define ULTRA_MOD_4				0x27
#define ULTRA_MOD_5				0x28
#define ULTRA_MOD_6				0x29
#define ULTRA_MOD_7				0x2A

#define PSLCD_MODULE			0x34
#define ULTRA_SENSOR			0x35

#define GYRO_MODULE 			0x69
#define ACCELEROMETER_MODULE 	0x1D
#define COMPASS_MODULE			0x21

/* Universal TWI command calls */
#define TWI_ID					0x30
#define TWI_SET_ADR				0x31
#define TWI_DIS_ADR				0x32

extern volatile unsigned char twiData[TWI_MAX_BUF];
extern volatile unsigned int twiMsgSize;
extern volatile unsigned char twiMsgLength;
extern volatile unsigned char twiDataInBuffer;
extern volatile unsigned char twiState;

extern void network_control(unsigned char);
//extern inline unsigned char twiBusy(void);
extern unsigned char twiBusy(void);
extern void twiMasterInit (unsigned long);
extern void twiSlaveInit(char);

#endif
