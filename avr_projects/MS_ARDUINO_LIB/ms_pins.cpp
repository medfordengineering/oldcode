/* -------------------------------------------------------------
Machine Science API
 
(c) Copyright Machine Science, 2012.  All rights reserved

Version Date: Mon Oct 25 11:03:19 EDT 2012
-----------------------------------------------------------------
============================================================== */

#include <ms_pins.h>

void output_pin(unsigned char pin)
{

	volatile unsigned char *reg;

	unsigned char bit = maskPin(pin);
	unsigned char port = portPin(pin);
	
	reg = portRegister(port);

	*reg |= bit;
	
}

void input_pin(unsigned char pin)
{
	volatile unsigned char *reg;

	unsigned char bit = maskPin(pin);
	unsigned char port = portPin(pin);
	
	reg = portRegister(port);

	*reg &= ~bit;
	
}

void high_pin(unsigned char pin)
{

	volatile unsigned char *out;

	unsigned char bit = maskPin(pin);
	unsigned char port = portPin(pin);
	
	out = outputPort(port);
	
	*out |= bit;
}

void low_pin(unsigned char pin)
{
	volatile unsigned char *out;
	
	unsigned char bit = maskPin(pin);
	unsigned char port = portPin(pin);
	

	out = outputPort(port);

	*out &= ~bit;
}

int pin_value(unsigned char pin)
{

	unsigned char bit = maskPin(pin);
	unsigned char port = portPin(pin);

	if (*inputPort(port) & bit) 
	return HIGH;
	else
	return LOW;
}

void pullup_on(unsigned char pin)
{

	volatile unsigned char *out;

	unsigned char bit = maskPin(pin);
	unsigned char port = portPin(pin);
	
	out = outputPort(port);
	
	*out |= bit;
}

void pullup_off(unsigned char pin)
{
	volatile unsigned char *out;
	
	unsigned char bit = maskPin(pin);
	unsigned char port = portPin(pin);
	

	out = outputPort(port);

	*out &= ~bit;
}

