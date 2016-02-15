#define NO_STATE				0
#define CAPTURE_GGA				1
#define WAIT_GGA				2 
#define PARSE_GGA				3
#define CAPTURE_RMC				4
#define WAIT_RMC				5
#define PARSE_RMC				6
#define CAPTURE_ADC				7	
#define FLASH_RECORD			8

#define SLAVE_ADDRESS			0x0B

#include <machinescience.h>
#include <twi_slave.c>
#include <timers.c>
#include <usart.c>
#include <gps_sensor.c>
#include <adc_sensor.c>
#include <analog.c>

/* FOR TESTING */
//#include <usart_print.c>
//#include <ms_print.c>
//#include <delay.c>

volatile unsigned char gpsConvertState = NO_STATE;

/* For now this should not be changed. */
volatile unsigned char gpsState = ENABLED;

void transmit_integer(signed int sensorValue)
{
	/* Transmit instant sensor values as two bytes */
	twiData[0] = sensorValue;
	twiData[1] = (sensorValue >> 8);
}
int main(void)
{	
	signed int sensor_value;

	/* Initialize slave */
	twiSlaveInit(SLAVE_ADDRESS);

	/* Initialize USART */
//	usart_init(4800);

	/* Initialize GPS */
//	gps_init();
	
	/* Initialize interval clock interrupt */	
//	interval_clock_init(SLOW); 

	/* Initialize analog converter */
//	adc_init();

	/* Initialize global interrupts */
	sei();


	while (TRUE) {
		if (twiDataInBuffer) {
			switch (twiData[0]) {
				/* Sound */
				case 0:
					sensor_value = adc_read(0);
					transmit_integer(sensor_value);
					break;
				/* X axis */
				case 1:
		//			sensor_value = adc_read(1);
		//			sensor_value = convert_acceleration(sensor_value);
		//			transmit_integer(sensor_value);
					transmit_integer(956);
					break;
				/* Y axis */
				case 2:
					sensor_value = adc_read(2);
					sensor_value = convert_acceleration(sensor_value);
					transmit_integer(sensor_value);
					break;
				/* Z axis */
				case 3:
					sensor_value = adc_read(3);
					sensor_value = convert_acceleration(sensor_value);
					transmit_integer(sensor_value);
					break;
				/* Temperature */
				case 4:
					sensor_value = adc_read(6);
					sensor_value = convert_temperature(sensor_value);
					transmit_integer(sensor_value);
					break;
				/* Light */
				case 5:
					sensor_value = adc_read(7);
					transmit_integer(sensor_value);
					break;
				/* Returns GPS values */
				case 6:
					twiData[0] = gpsParsed[twiData[1]];
					twiData[1] = gpsParsed[twiData[1] + 1];
					break;
			}
			TWCR = TWISLAVEACTIVEACK;
			twiDataInBuffer = FALSE;
		}
	}
}
