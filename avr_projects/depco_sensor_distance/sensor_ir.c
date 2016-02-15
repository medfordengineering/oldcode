#define SMALL_BUFFER
#define WDTO_250MS		4

#define LEDPRT			PORTD
#define LEDDIR			DDRD
#define LEDSTARTPIN		0

#include <machinescience.h>
#include <twi_masterslave.cpp>
#include <ms_delay.cpp>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <depco_ir.h>

unsigned char filterRate;
unsigned int filterCount;

void wdt_init(char state, char count)
{
	cli();

	/* Reset to avoid RESET during change */
	wdt_reset();
	
	if (state == ENABLE) 

		/* Enable WDT for the time period specified in count */
		wdt_enable(count);
	else if (state == DISABLE) 

		/* Turn off WDT */
		wdt_disable();

	sei();
}

void sensor_init(char state)
{
	if (state == ENABLE) {
		clear_bit(DDRC, 0);
		ADMUX = 	(1<<REFS0)| 	//Internal reference
					(0<<ADLAR)|		//8-bit resolution CHECK THIS?
					(0<<MUX0);		//channel zero
		ADCSRA = 	(7<<ADPS0)|  	//128 prescale
					(1<<ADEN);		//enable adc
	}
	else if (state == DISABLE) 
		ADCSRA = (0<<ADEN);
}

/* Initialize address LEDs */
void led_init(void)
{
	/* Set TWI address LED pins as outputs */
	LEDDIR |= (7<<LEDSTARTPIN);

	/* Turn off TWI port LED pins */
	LEDPRT |= (7<<LEDSTARTPIN);
}

/* Turns on or off address LEDs */
void led_address(char state)
{
	char address;
	/* Clear LED port pins - turns on all LEDs */
	LEDPRT &= ~(7<<LEDSTARTPIN);
	if (state == ON) {
		/*Retreive absolute address and subtract base address to get relative address */
		address = eeprom_read_byte((unsigned char *) TWIADDR_EEPROM) - IRD_MOD_0;
		/*Displays relative address on LEDs */
		PORTD |= ~(address<<LEDSTARTPIN);
	}
	else if (state == OFF) 
		/* Turn off LED address */
		PORTD |= (7<<LEDSTARTPIN);
}

unsigned int sensor_read(void)
{
	//USED?
//	static unsigned int adc_value;

	/* Start analog conversion */
	set_bit(ADCSRA, ADSC);

	/* Wait for analog conversion to complete */
	while (test_bit(ADCSRA, ADSC));

	return ADCW;
}

unsigned int avg_filter(void)
{
	/* Big value to sum many samples */
	unsigned long value = 0;
	int x;

	/* Grab samples from ADC based on count */
	for (x = 0; x < filterCount; x++)
		value += sensor_read();

	/* Return average value */
	return value /= filterCount;
}

unsigned int pass_filter(unsigned int newValue)  
{
	static unsigned int lastValue;

	/* Run low pass filter on repeated values */ 
	lastValue = ((100 - filterRate)*lastValue + (filterRate*newValue)) / 100;

	return lastValue;
}

int main (void)
{
	unsigned int sensor;
	char filterType, address;

	/* Set intial filter type as average */
	filterType = AVERAGE_FILTER;

	/* Set initial filter rate */
	filterRate = 20;

	/* Set initial count rate */
	filterCount = 100;

	/* Turn on analog converter */
	sensor_init(ENABLE);

	/* Writes default address to EEPROM: This should be set in manufacture but is here for testing
	 * purposes. The EEPROM no rewrite fuse needs to be set */
	//eeprom_write_byte((unsigned char *)TWIADDR_EEPROM, IRD_MOD_7);

	/* Initialize LED ports */
	led_init();

	/* Get TWI address from EEPROM and display it */
	led_address(ON);

	/* Set device as slave */
	twiState = TWI_SLAVE;

	/*On start up, retrieve address from EEPROM and initialize twi */
	twiSlaveInit(eeprom_read_byte((unsigned char *) TWIADDR_EEPROM));

	/* Turn on watch dog timer */
	wdt_init(ENABLE, WDTO_250MS);

	sei();

	while (TRUE) {
		wdt_reset();
		if (twiDataInBuffer) {
			switch (twiData[0]) {
				case CMD_IRSEN_READ:
					if (filterType == AVERAGE_FILTER)
						sensor = avg_filter();
					//else if (filterType == PASS_FILTER)
					//	sensor = pass_filter();
					twiData[0] = sensor >> 8;
					twiData[1] = sensor;
				break;
				case CMD_IRSEN_SETTWIADR:
					address = twiData[1];
					if (address < 1) address = 1;			// Prevents illegal address
					if (address > 7) address = 7; 			// Prevents illegal address
					eeprom_write_byte((unsigned char *)TWIADDR_EEPROM, twiData[1]);
					twiSlaveInit(eeprom_read_byte((unsigned char *) TWIADDR_EEPROM));
					led_address(ON);
				break;
				case CMD_IRSEN_DISPLAYTWIADR:
					led_address(twiData[1]);
				break;
				case CMD_IRSEN_WDSTATE:
					wdt_init(twiData[1], twiData[2]);
				break;
				case CMD_IRSEN_SETSTATE:
					sensor_init(twiData[1]);
				break;
				case CMD_IRSEN_FILTERCOUNT:
					filterCount = twiData[1] + (8 << twiData[2]);
					if (filterCount > 1000) filterCount = 1000; 	//Prevents overshoot
					if (filterCount < 1) filterCount = 1;			//Prevents undershoot
				break;
			}
			TWCR = TWISLAVEACTIVEACK;
			twiDataInBuffer = FALSE; 
		}
	}
}
