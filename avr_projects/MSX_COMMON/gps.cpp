#include <gps.h>

#define GPS_BUFFER_SIZE 			90
char gpsRaw[GPS_BUFFER_SIZE];

#define PARSED_BUFFER_SIZE			90
unsigned char gpsParsed[PARSED_BUFFER_SIZE];

volatile int gpsIndex;

void gps_send_msg(const char *);
void gps_parse_string(uint8_t, uint8_t, uint8_t, uint8_t);
void gps_parse_gga(void);
void gps_parse_rmc(void);

//volatile unsigned char gpsCaptureState = FALSE;
volatile unsigned char gpsCaptureState = 0;

signed int gps_data(int value)
{
	gpsCaptureState = 0;

	if (value < 20) {
		gps_send_msg(GPS_QRY_GGA_MSG);
		//while (gpsCaptureState);
		while(gpsCaptureState < 2);
		gps_parse_gga();
	}
	else {
		gps_send_msg(GPS_QRY_RMC_MSG);
		//while (gpsCaptureState);
		while(gpsCaptureState < 2);
		gps_parse_rmc();
	}
	return (gpsParsed[value + 1] << 8) + gpsParsed[value];
}

ISR (USART_RX_vect)
{
	char c;

	/* Get character from USART */
	c = UDR0;

	/* Check for end of string */
	if (c == '\n') {
		gpsIndex = 0;
	//	gpsCaptureState = FALSE;
		gpsCaptureState++;
	}

	/* Store character in buffer */
	else 
		gpsRaw[gpsIndex++] = c;
}

void gps_init(void)
{
	unsigned char baud;

	/* Set baud rate */
	baud = (unsigned char) (FOSC/(16L * 4800)) - 1;

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
//	while (usart_read() != '$')

	/* Disable all GPS strings */
	gps_send_msg(GPS_DIS_GGA_MSG);
	gps_send_msg(GPS_DIS_GLL_MSG);
	gps_send_msg(GPS_DIS_GSA_MSG);
	gps_send_msg(GPS_DIS_GSV_MSG);
	gps_send_msg(GPS_DIS_RMC_MSG);
	gps_send_msg(GPS_DIS_VTG_MSG);
	
	sei();
}

void gps_send_msg(const char *msg)
{
    usart_const_text(msg);
    usart_const_text(GPS_END_MSG);
}

void gps_parse_string(uint8_t value, uint8_t offset, uint8_t length, uint8_t place)
{
	char comma = 0;
	char buffer[6];
	signed int index = 0, count = 0, temp = 0;

	/* Count commas to find GPS value location */
	do {	
		if (gpsRaw[index++] == ',')
			comma++;
	} while (comma <= value);

	/* Store GPS value in buffer with null termination */
	for (count = 0; count < length; count++) {
		if ((buffer[count] = gpsRaw[index + offset + count]) == ',')
			break;
	}
	buffer[count] = '\0';

	/* Convert GPS value to signed integer */
	temp = atoi(buffer);

	/* Use negative values in place of directional designator */
	if ((place == 6) && (offset == 0) && (gpsRaw[28] == 'S')) 
		temp *= -1;

	if ((place == 10) && (offset == 0) && (gpsRaw[41] == 'W')) 
		temp *= -1;

	/* Store GPS value as two bytes */
	gpsParsed[place] = temp;
	gpsParsed[place + 1] = temp >> 8;
}

void gps_parse_gga(void)
{
	gps_parse_string(0,0,2,0);		// hours
	gps_parse_string(0,2,2,2);		// minutes
	gps_parse_string(0,4,2,4);		// seconds
	gps_parse_string(1,0,4,6);		// latitude, high
	gps_parse_string(1,5,4,8);		// latitude, low
	gps_parse_string(3,0,5,10);		// longitude, high
	gps_parse_string(3,6,4,12);		// longitude, low
	gps_parse_string(5,0,1,14);		// position fix indicator
	gps_parse_string(6,0,2,16);		// satellites used
	gps_parse_string(8,0,8,18);		// altitude
}

void gps_parse_rmc(void)
{
	gps_parse_string(6,0,5,20);		// speed over ground
	gps_parse_string(7,0,3,22);		// course over ground
	gps_parse_string(8,0,2,24);		// day 
	gps_parse_string(8,2,2,26);		// month 
	gps_parse_string(8,4,2,28);		// year
}

