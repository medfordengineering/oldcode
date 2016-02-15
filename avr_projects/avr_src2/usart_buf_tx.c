/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2007.  All rights reserved

 Version Date: Tue Jan 15 17:05:20 EST 2008
-----------------------------------------------------------------
============================================================== */

#include "usart.h"

void usart_interrupt_tx(unsigned char value)
{
	/* Enable/disable interrupt for transmitter */
	if (value == ENABLE)
		set_bit(UCSR0B, TXCIE0);
	else
		clear_bit(UCSR0B, TXCIE0);
}

SIGNAL (SIG_USART_TRANS)
{
	if (TX_counter != 0){

		if(fPrimedIt == 1){
			fPrimedIt = 0;

			if(++TX_Rd_Index > TX_BUFFER_SIZE)		// Test buffer and wrap
				TX_Rd_Index = 0;
		}
		TX_Counter--;								// Keep track of counter
	}				

	if(TX_Counter != 0){
		UDR0 = TX_Buffer[TX_Rd_Index];

		if(++TX_Rd_Index > TX_BUFFER_SIZE)
			TX_Rd_Index = 0;

		TX_Counter--;
	}
}

UCR0 |= 0x40


