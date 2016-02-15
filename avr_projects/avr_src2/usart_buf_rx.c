/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date: Tue Jan 15 17:05:20 EST 2008
-----------------------------------------------------------------
============================================================== */

#include <usart.h>

char validData = FALSE;

ISR (USART_RX_vect)
{
	char c;

	c = UDR0; 							

	if (c == 0xFE) validData = TRUE;	// Indicate start byte received

	rxBuffer[rxWriteIndex] = c;			// Place character the buffer
	
	if(++rxWriteIndex > RX_BUFFER_SIZE)	// Wrap the pointer
		rxWriteIndex = 0;

	if(++rxCounter > RX_BUFFER_SIZE){	// If rxCounter gets bigger than
		rxCounter = RX_BUFFER_SIZE;		// the size of the buffer you have
		rxBufferOverflow = 1;			// an overflow problem. 
	}
}

char usart_buffer_read(void)
{
	char c;
	//int i = 0;

	while(rxCounter == 0);				// Wait for the next new character
//		if (i++ > 2000) return -1;		// Time out if no new characters - breaks xbee commmand mode

	c = rxBuffer[rxReadIndex];			// Get character from buffer	
	
	if(++rxReadIndex > RX_BUFFER_SIZE)	// Wrap pointer	
		rxReadIndex = 0;

	if(rxCounter)						// Indicate removal of character
		rxCounter--;

	return c;
}

void usart_interrupt_rx(unsigned char value)
{
	/* Enable/disable interrupt for receiver */
	if (value == ENABLE)
		set_bit(UCSR0B, RXCIE0);
	else
		clear_bit(UCSR0B, RXCIE0);
}
