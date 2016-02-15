/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Wed Oct 21 12:14:38 EDT 2009
-----------------------------------------------------------------
============================================================== */

#include <twi_master.h>

volatile unsigned char twiData[TWI_MAX_BUF];
volatile unsigned int twiMsgSize;
static unsigned int twiCount;

void network_control(unsigned char state)
{
	twiMasterInit(175000);
	sei();
}

/* Checks to see of TWI interrupt is enabled. This is defined as busy by all the modes used in the ISR. */
inline unsigned char twiBusy(void)
{
	return (TWCR & (1<<TWIE)) | (TWCR & (1 << TWSTO));   
}

void twiMasterInit (unsigned long sclFrequency)
{
	unsigned int prescale[4] = {1, 4, 16, 64};
	unsigned int i = 0;
	unsigned int twbrValue;
	
	do {
		twbrValue = ((FOSC / sclFrequency ) - 16) / (2 * prescale[i++]);

	} while ((twbrValue > 0xFF) || (twbrValue < 12));

	TWBR = twbrValue;
	TWSR &= 0xFC;
	TWSR |= (i - 1);

	TWCR = TWIMASTERPASSIVE;
}

ISR (TWI_vect)
{
	switch (TWSR & 0xF8){

		/* The Master has sent a START and will load the Slave address with r/w bit into TWDR in preparation
		 * for transmitting. */
		case TWI_START:
			twiCount = 0;
			TWDR = twiData[twiCount++];
		case TWI_RESTART:
			TWCR = TWIMASTERACTIVE;	
		break;

		/* The Master has sent the Slave address with a write bit and an ACK has been received from the Slave.
		 * The first byte of data is place in TWDR. */
		case SLAW_ADR_ACK:
			TWDR = twiData[twiCount++];
			TWCR = TWIMASTERACTIVE;	
		break;

		/* The Master has sent the Slave address with the write bit and a NACK has bee received from the Slave
		 * (or no exisiting device). The Master then attempts a RESTART. */
		case SLAW_ADR_NACK:
			TWCR = TWIMASTERSTART;
		break;

		/* The Master has sent the Slave address with the read bit and a ACK has been received from the Slave.
		 * The Master will now expect to receive data from the Slave. */
		case SLAR_ADR_ACK:
			TWCR = TWIMASTERACK;
		break;

		/* The Master has sent the Slave address with the read bit and a NACK has been received from the Slave
		 * (or no existing device). The Master then attempts a RESTART. */
		case SLAR_ADR_NACK:
			TWCR = TWIMASTERSTART;
		break;

		/* The Master has sent one or more bytes of data to the Slave and received an ACK. If the last byte
		 * has been sent, the Master will initiate a STOP. */
		case SLAW_DAT_ACK:
			if (twiCount < twiMsgSize + 1){
				TWDR = twiData[twiCount++];
				TWCR = TWIMASTERACTIVE;
			}
			else
				TWCR = TWIMASTERSTOP;
		break;

		/* The Master has receieved one more more bytes of data from the Slave and returned an ACK.  If this
		 * is the last byte expected, the Master will return a NACK on the next byte. */
		case SLAR_DAT_ACK:
			twiData[twiCount++] = TWDR;
			if (twiCount < twiMsgSize)
				TWCR = TWIMASTERACK;
			else
				TWCR = TWIMASTERNACK;
		break;

		/* The Master has receieved the last byte from the Slave and returned a NACK. The Master now intiates
		 * a STOP. */
		case SLAR_DAT_NACK:
			twiData[twiCount] = TWDR;
			TWCR = TWIMASTERSTOP;
		break;

		case SLAW_DAT_NACK:
		case TWI_BUS_ERROR:
		default:
			TWCR = TWIMASTERPASSIVE;
	}
}

