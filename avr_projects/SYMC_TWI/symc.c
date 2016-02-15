#include <machinescience.h>
#include <usart.c>
#include <spi_master.c>
#include <at45.c>
#include <ms_print.c>
#include <usart_print.c>
#include <adc.c>

#include <delay.c>

#define MHZ8	0
#define KHZ125	6
#define NOTHING 9

volatile unsigned char minutes;
volatile unsigned char hours;
volatile unsigned char days;
volatile unsigned char light;
volatile unsigned char temp;

unsigned char dataSet[512];
volatile unsigned char secondsCounter = 0;
volatile unsigned char bufferIndex = 0;
volatile unsigned char pageIndex = 0;

ISR (USART_RX_vect)
{
	char c;
	c = UDR0;
	if (c == 'S') {
		//get next value
		//put data into buffer
		//send data to usart
	}
}


//Every 2 seconds
ISR (TIMER2_OVF_vect)
{
	//change up clock speed
	//enable usart interrupt

	//Every Minute
	if (secondsCounter++ > 30) {
		secondsCounter = 0;

		//Read sensors
		light = adc_read(7);
		temp = adc_read(6);

		//Store time/data in local buffer
		dataSet[bufferIndex++] = minutes;
		dataSet[bufferIndex++] = hours;
		dataSet[bufferIndex++] = days;
		dataSet[bufferIndex++] = light;
		dataSet[bufferIndex++] = temp;

		//Every 102 Minutes
		if (bufferIndex > 510) {
			bufferIndex = 0;

			//Move local buffer to external memory 
			at45_buffer_write(0, dataSet, 510);  
			at45_page_write(pageIndex++);
		}
		minutes++;

		//Every Hour
		if (minutes > 60) {
			minutes = 0;
			hours++;

			//Every Day
			if (hours > 24) { 
				hours = 0;
				days++; 
			}
		}
	}
//delay
//disable usart interrupt
//change down clock speed
}

void changeClock(char speed)
{
	cli();
	CLKPR = (ENABLE<<CLKPCE);
	CLKPR = (speed<<CLKPS0);
	sei();
}

void interval_clock_init(void) 
{
	/* Enable asynchonous mode for timer2 */
	set_bit(ASSR, AS2);

	/* Enable 128 prescale */
	set_bit(TCCR2B, CS20);
	set_bit(TCCR2B, CS22);	

    /* Enable timer2 interrupt on overflow */
    set_bit(TIMSK2, TOIE2);

    /* Clear timer2 overflow flag */
    set_bit(TIFR2, TOV2);
}

signed int convert_temperature( signed int sensorValue)
{
	/* CHECK ACCURACY 3 ~ 3.22 */
	return ((3 * sensorValue) - 600) / 10;
}

void usartInit(void)
{
	//Set baud rate
	UBRR0L = 1;
	//Turn or receive and transmit 
	UCSR0B =  (ENABLE<<RXEN0) | (ENABLE<<TXEN0);
	//Set to 8 bits
	UCSR0C = (3<<UCSZ00);
}

int main(void)
{	
	/*Change Baud Rate */
	usartInit();

	/* Initialize analog converter */
	adc_init();
	adc_resolution(ADC_10BIT); //check 8 bit!

	/* Set up 2 second Clock */
	interval_clock_init();

	sei();

	while(1);
}

