#include <gps.h>

//unsigned char gpsParsed[PARSED_BUFFER_SIZE];
//volatile unsigned char gpsCaptureState;

signed int mtk_data(int value)
{
	gpsCaptureState = 0;

	if (value < 20) {
		gps_send_msg(MTK_QRY_GGA_MSG);
		while(gpsCaptureState < 2);
		gps_parse_gga();
	}
	else {
		gps_send_msg(MTK_QRY_RMC_MSG);
		while(gpsCaptureState < 2);
		gps_parse_rmc();
	}
	return (gpsParsed[value + 1] << 8) + gpsParsed[value];
}

void mtk_init(void)
{
	unsigned char baud;

	/* Set baud rate */
	baud = (FOSC/(16L * 38400UL)) - 1;

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

	/* Wait until GPS begins transmitting data */
	while (usart_read() != '$')

	/* Disable all GPS strings */
	gps_send_msg(GPS_DIS_MSG);
	
	sei();
}

