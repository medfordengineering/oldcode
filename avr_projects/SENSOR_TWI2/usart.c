#include <usart.h>

void usart_init(unsigned long baud)
{	
	/* Set baud rate */
	baud = (FOSC/(16L * baud)) - 1;

	/* Set baud rate registers */
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;

	/* Enable USART receiver */
	set_bit(UCSR0B, RXEN0); 		

	/* Enable USART transmitter */
	set_bit(UCSR0B, TXEN0);			

	/* Enable interrupt for usart receiver */
	set_bit(UCSR0B, RXCIE0);	

	/* Set USART for 8-bit */
	UCSR0C = (3<<UCSZ00);		
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
    while (pgm_read_byte(s) != '\0') {
		usart_write(pgm_read_byte(s));
		s++;
    }
}

void usart_text2(char *s){
	while(*s) {
		usart_write(*s);
		s++;
	}
}
