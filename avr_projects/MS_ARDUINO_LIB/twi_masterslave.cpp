/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Wed Oct 10 12:51:15 EDT 2012

-----------------------------------------------------------------
============================================================== */

#include <twi_masterslave.h>

volatile unsigned char twiData[TWI_MAX_BUF];
volatile unsigned int twiMsgSize;
static unsigned int twiCount;
volatile unsigned char twiState;
volatile unsigned char countAdrNack;

volatile unsigned char twiMsgLength;
volatile unsigned char twiDataInBuffer;

void network_control(unsigned char state)
{
	twiMasterInit(175000);
	sei();
}

/* Checks to see of TWI interrupt is enabled. This is defined as busy by all the modes used in the ISR. */
//inline unsigned char twiBusy(void)
unsigned char twiBusy(void)
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
	if (twiState == TWI_MASTER) {
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
				countAdrNack++;
				TWCR = TWIMASTERSTART;
			break;

			/* The Master has sent the Slave address with the read bit and a ACK has been received from the Slave.
			 * The Master will now expect to receive data from the Slave. */
			case SLAR_ADR_ACK:
				countAdrNack = 0;
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
	else if (twiState == TWI_SLAVE) {
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
}
