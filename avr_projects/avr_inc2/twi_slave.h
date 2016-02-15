/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue Mar 31 15:31:50 EDT 2009
-----------------------------------------------------------------
============================================================== */
#ifndef _TWIS_H_
#define _TWIS_H_

//#include "machinescience.h"
#include <ms.h>

//FOR DEBUGGING
//#include "usart.h"
//#include "usart_init.c"
//#include "usart_rx.c"
//#include "usart_tx.c"
//#include "usart_write.c"

#define SLAW_ADDR_ACK	0x60
#define SLAW_DATA_ACK   0x80
#define SLAW_STOP		0xa0
#define SLAR_ADDR_ACK  	0xa8
#define SLAR_DATA_ACK	0xb8
#define SLAR_LB_NACK	0xc0
#define SLAR_LB_ACK		0xc8

/* This should be defined once for both master and slave. */
#define TWI_MAX_BUF		300

#define TWISLAVEACTIVEACK	(1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)                        
#define TWISLAVEACTIVENACK	(1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)                    
#define TWISLAVEPASSIVE		(1<<TWEN)|(0<<TWIE)|(0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)                      

extern volatile char twiData[TWI_MAX_BUF];
extern volatile char twiMsgLength;
extern volatile char twiDataInBuffer;

extern void twiSlaveInit(char);

#endif
