#define BAUD_9600		129
#define BAUD_19200		64
#define BAUD_38400		32

ISR (USART_RX_vect)
{
	char c;

	c = UDR0;

}

void usart_initialize(void)
{
	UCSR0B =	(1<<RXCIE0)|		// Enable USART receive interrupt
				(1<<RXEN0)|			// Enable USART receive
				(1<<TXEN0);			// Enable USART transmit

	UCSR0C = 	(3<<UCSZ00);		// Set USART for 8-bit

	UBRR0L = BAUD_9600;

}

void usart_write( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ((UCSR0A & (1 << UDRE0)) == FALSE);

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

void usart_text(const char *s)
{
    //while (pgm_read_byte(s) != 0x00) {
	//	usart_write(pgm_read_byte(s));
		s++;
//    }
}


