/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2007.  All rights reserved

 Version Date: Thu Nov  8 06:16:15 EST 2007
-----------------------------------------------------------------
============================================================== */

#include <ms_usart.h>

//void usart_init(unsigned long baud)
void usart_init(void)
{
	usart_control(ENABLED);
	//usart_baud(baud);
}

void usart_control(char state)
{
	if (state == ENABLED) {
		usart_baud(9600);
		usart_size(TRUE);
		usart_rx(ENABLED);
		usart_tx(ENABLED);
	}
}

void usart_baud( unsigned long baud )
{
	/* Set baud rate */
	baud = (FOSC/(16L * baud)) - 1;

	/* Set baud rate registers */
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;

}

void usart_size(unsigned char size)
{
	/* 	Set character size to 8-bit */
	UCSR0C = (3<<UCSZ00);
}

void usart_rx(unsigned char value)
{
	/* Enable/disable receiver */
	if (value == ENABLE)
		set_bit(UCSR0B, RXEN0);
	else
		clear_bit(UCSR0B, RXEN0);
}

void usart_tx(unsigned char value)
{
	/* Enable/disable transmitter */
	if (value == ENABLE)
		set_bit(UCSR0B, TXEN0);
	else
		clear_bit(UCSR0B, TXEN0);
}

void usart_write( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while(!(test_bit(UCSR0A, UDRE0)));

	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char usart_read( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );

	/* Get and return received data from buffer */
	return UDR0;
}

/* NOTE: This argument might work better as a const char */
void usart_text(char *s){
	while(*s) {
		usart_write(*s);
		s++;
	}
}

void usart_textln(char *s){
	while(*s) {
		usart_write(*s);
		s++;
	}
	usart_return();
}

void usart_const_text(const char *s)
{
    while (pgm_read_byte(s) != '\0') {
		usart_write(pgm_read_byte(s));
		s++;
    }
}

void usart_return(void)
{
	usart_write(CARRIAGE_RETURN);
	usart_write(LINE_FEED);
}

