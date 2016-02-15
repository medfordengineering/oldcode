/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2004.  All rights reserved

 Version Date: Mon Nov 26 07:59:55 EST 2007
-----------------------------------------------------------------
============================================================== */

/*NOTES: Assumes an 8mHz CPU clock
*/

#include "ms_remote.h"

unsigned char remoteFlag;
volatile unsigned char sony_ir_code;

signed char convert_remote(char);

/* IR REMOTE CONTROL */

#define MAX_IRCOUNT				22	

const unsigned char remote_codes[22] = 	{ 	145, 1, 129, 65, 193, 33, 161, 97, 225, 17, 169, 9, 
											137, 73, 201, 209, 89, 25, 57, 217, 153, 185
										};

void remote_init(void)
{
	/* Timer1 set for receiving signal from IR remote  */
//	timer1_prescale(TMR1_PRE64); 			// Timer1 with 8us interval (64pre at 8mHz)
	TCCR1B &= 0xF8;
	TCCR1B |= 0x03;

//	timer1_mode(TMR_MODE0);					// Normal mode full 16 bit count
	TCCR1A &= 0xF0;
	TCCR1B &= 0xC7;

	clear_bit(TCCR1B, ICES1); 				// Set for capture on falling edge
	set_bit(TIMSK1,ICIE1); 					// Set interrupt for input capture unit 
	set_bit(TCCR1B, ICNC1); 				// Add noise canceller for input capture

	/* Set up input capture pin */
	clear_bit(DDRB, 0);						// Set input caputure pin for input
	set_bit(PORTB, 0);						// Turn on pull-up resistor
	 
	sei();									// Turn universal interrupts
}

ISR (TIMER1_CAPT_vect)
{
	static unsigned int saved;
	static unsigned char data;	
	static unsigned char count;
	unsigned int current;

	remoteFlag = TRUE;
	cli();
	/* Measure interval between each falling edge */
	current = ICR1;
	saved = current - saved;

	/* Check if interval is a start pulse */
	if ((saved > IR_START) && (saved < (IR_START + IR_OFFSET))) 
		count++;

	/* If previous start pulse, check for a zero pulse */
	else if ((saved > IR_ZERO) && (saved < (IR_ZERO + IR_OFFSET)) && (count)){
		data <<= 1;
		count++;
	}

	/* If previous start pulse, check for a one pulse */
	else if ((saved > IR_ONE) && (saved < (IR_ONE + IR_OFFSET)) && (count)){
		data <<= 1;
		data |= 0x01;
		count++;
	}

	/* If not start, one or zero than reset and look for start */
	else
		count = 0;

	/* Capture first 8 bits from remote and reset to look for start */
	if (count > 8){
		count = 0;
		sony_ir_code = data;
	}
	saved = current;
	sei();
}

unsigned char remote_read(void)
{
	/* Candidate for local variable */
	return convert_remote(sony_ir_code);
}

/* This should be removed from lcd_twi, since it now resides here. */
signed char convert_remote(char code)
{
	signed char i = 0;
	while ((code != remote_codes[i++]) && (i <= MAX_IRCOUNT));
	if (i > MAX_IRCOUNT) i = 0;
	return (i - 1);
}
