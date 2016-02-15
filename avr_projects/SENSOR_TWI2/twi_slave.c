/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2008.  All rights reserved

Version Date: Wed Oct 21 17:09:40 EDT 2009
-----------------------------------------------------------------
============================================================== */

#include "twi_slave.h"

volatile char twiData[TWI_MAX_BUF];
volatile char twiMsgLength;
volatile char twiDataInBuffer;
static int twiCount;

/* This will set the slave address allow for general calls and 
 * initialize the TWI as a slave module to receive requests. We could
 * initialize it in passive mode alternatively. */
void twiSlaveInit(char address)
{
	/* Set Slave address and enable general calls */
	TWAR = ((address << 1) | (TRUE));

	/* Enable Slave to receive requests from Master */
	TWCR = TWISLAVEACTIVEACK;	

	//FOR DEBUGGING 
	//usart_init(19200);
}

ISR (TWI_vect)
{
	//FOR DEBUGGING
	//usart_write(TWSR);
	
	switch (TWSR & 0xF8){

		/* If you reach this point, the slave has been correctly addressed with the data direction
		 * bit clear for a write. This also confirms that the Slave has ACKed the ninth bit of the 
		 * transmission to tell the Master that the address has been received. twiSlaveActiveACK
		 * clears the interrupt and prepares the slave to ACK the expected first data byte. */
		case SLAW_ADDR_ACK:
			TWCR = TWISLAVEACTIVEACK;
		break;

		/* If you reach this point, the slave has already been addressed correctly and has
		 * received one or more bytes of data. This state will continue to receive bytes of data
		 * until the STOP is sent or the number of bytes exceeds TWI_MAX_BUF. After each byte is 
		 * received twiSlaveActiveACK is called to prepare the slave to recieve the next byte. */
		case SLAW_DATA_ACK:
			if (twiCount < TWI_MAX_BUF){
				twiData[twiCount++] = TWDR;
				TWCR = TWISLAVEACTIVEACK;	
			}
				//twiState = TWIMAXBUFFERROR;
		break;

		/* If you reach this point, the master has sent a STOP. At this point the twiSlave is turned
		 * off temporarily so that user code can process the data that has been received. The buffer
		 * counter is also reset to zero. Once the data has been processed user code can 
		 * re-enable the twi with a twiSlaveActiveACK. */
		case SLAW_STOP:
			twiMsgLength = twiCount;
			twiCount = 0;
			twiDataInBuffer = TRUE;
			TWCR = TWISLAVEPASSIVE;
		break;	

		/* If you reach this point, the slave has been correctly addressed with the data direction
		 * bit set for a read. Again, the Slave has correctly ACKed the ninth bit. The first
		 * byte in twiData is now put into TWDR for transmission back to the Master. */
		case SLAR_ADDR_ACK:
			TWDR = twiData[twiCount++];
			TWCR = TWISLAVEACTIVEACK;
		break;

		/* If you reach this point, the slave has previous been addressed correctly with a read
		 * bit and now has sent back at least one byte of data. The slave will now continue to send
		 * back bytes until it reaches TWI_MAX_BUF and expect a NACK from the Master. */
		case SLAR_DATA_ACK:
			if (twiCount < TWI_MAX_BUF){
				TWDR = twiData[twiCount++];
				TWCR = TWISLAVEACTIVEACK;	
			}
//			else{
//				twiSlaveActiveNACK();
//			}
		break;

		/* If you reach this point the last data byte has been transmitted and a NACK has been 
		 * received.  */
		case SLAR_LB_NACK:
			TWCR = TWISLAVEACTIVEACK;	
			twiCount = 0;

		break;

	}
}

