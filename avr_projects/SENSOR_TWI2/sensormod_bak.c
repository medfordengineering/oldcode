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
//#include <dataflash.c>
#include <spi_master.c>
#include <at45.c>

/* FOR TESTING */
#include <delay.c>
#include <usart_print.c>
#include <ms_print.c>

volatile unsigned char gpsConvertState = NO_STATE;

/* For now this should not be changed. */
volatile unsigned char gpsState = ENABLED;
volatile unsigned char loggingState = DISABLED;
//volatile unsigned char loggingState = ENABLED;
unsigned int pageIndex;

void transmit_integer(signed int sensorValue)
{
	/* Transmit instant sensor values as two bytes */
	twiData[0] = sensorValue;
	twiData[1] = (sensorValue >> 8);
}

void write_to_flash(void)
{
	static char bufferIndex;

	if (loggingState == ENABLED) {
		at45_buffer_write(bufferIndex, gpsParsed, 42);
		
		bufferIndex += 42;
		if (bufferIndex > 210) {
			at45_page_write(pageIndex++);
			bufferIndex = 0;
		}
	}
}

int main(void)
{	
	signed int sensor_value;

	/* Initialize slave */
	twiSlaveInit(SLAVE_ADDRESS);

	/* Initialize USART */
	usart_init(4800);

	/* TESTING AREA */
	spi_master_init();

	/* Initialize GPS */
	gps_init();
	
	/* Initialize interval clock interrupt */	
	interval_clock_init(SLOW); 

	/* Initialize analog converter */
	adc_init();

	/* Initialize SPI transfer */
	spi_master_init();

	/* Initialize global interrupts */
	sei();

	while (TRUE) {
		switch (gpsConvertState) {
			/* Request GGA data */
			case CAPTURE_GGA:
				gps_send_msg(GPS_QRY_GGA_MSG);
				gpsConvertState = WAIT_GGA;
				break;
			/* Parse GGA data */
			case PARSE_GGA:
				gps_parse_gga();
				gpsConvertState = CAPTURE_RMC;
				break;
			/* Request RMC data */
			case CAPTURE_RMC:
				gps_send_msg(GPS_QRY_RMC_MSG);
				gpsConvertState = WAIT_RMC;
				break;
			/* Parse RMC data */
			case PARSE_RMC:
				gps_parse_rmc();
				gpsConvertState = CAPTURE_ADC;
				break;
			/* Capture analog data */
			case CAPTURE_ADC:
				record_analog();
				gpsConvertState = FLASH_RECORD;
				//gpsConvertState = NO_STATE;
				break;
			/* Record all data to flash */
			case FLASH_RECORD:
				write_to_flash();
				gpsConvertState = NO_STATE;
				break;
		}	
		if (twiDataInBuffer) {
			switch (twiData[0]) {
				/* Sound */
				case 0:
					sensor_value = adc_read(0);
					transmit_integer(sensor_value);
					break;
				/* X axis */
				case 1:
					sensor_value = adc_read(1);
					sensor_value = convert_acceleration(sensor_value);
					transmit_integer(sensor_value);
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
				/* Starts and stops data logging */
				case 7:
					if (twiData[1] == ENABLED) {
						loggingState = ENABLED;
					}
					else 
						loggingState = DISABLED;
					break;
				case 8:
				/* Reads specific range of recorded data */
					at45_page_read(twiData[1], twiData[2], twiData, 42);
					break;
			}
			TWCR = TWISLAVEACTIVEACK;
			twiDataInBuffer = FALSE;
		}
	}
}
