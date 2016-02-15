/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   13 June 2008
-----------------------------------------------------------------
============================================================== */

#include "pins.h"

const unsigned char PROGMEM port_to_mode_PGM[] = {
	&PINB, //warning: initialization makes integer from pointer without a cast
	&PINC,
	&PIND,
};

void input_pin (unsigned char pinRef)
{
	volatile unsigned char * reg;
	unsigned char pin = pinRef % 8;

	reg = portModeRegister(pinRef / 8);

	reg++;

	*reg &= ~(1 << pin);
}

void output_pin(unsigned char pinRef)
{
	volatile unsigned char * reg;
	unsigned char pin = pinRef % 8;

	reg = portModeRegister(pinRef / 8);

	reg++;

	*reg |= 1 << pin;
}

void high_pin(unsigned char pinRef)
{
	volatile unsigned char * reg;
	unsigned char pin = pinRef % 8;

	reg = portModeRegister(pinRef / 8);

	reg += 2;

	*reg |= 1 << pin;
}

void low_pin(unsigned char pinRef)
{
	volatile unsigned char * reg;
	unsigned char pin = pinRef % 8;

	reg = portModeRegister(pinRef / 8);

	reg += 2;

	*reg &= ~(1 << pin);
}

void toggle_pin(unsigned char pinRef)
{
	volatile unsigned char * reg;
	unsigned char pin = pinRef % 8;

	reg = portModeRegister(pinRef / 8);

	reg += 2;

	*reg ^= (1 << pin);
}

void pullup_on(unsigned char pinRef)
{
	volatile unsigned char * reg;
	unsigned char pin = pinRef % 8;

	reg = portModeRegister(pinRef / 8);

	reg += 2;
	*reg |= 1 << pin;
	
}

void pullup_off(unsigned char pinRef)
{
	volatile unsigned char * reg;
	unsigned char pin = pinRef % 8;

	reg = portModeRegister(pinRef / 8);

	*reg &= ~(1 << pin);
	
}
char pin_value(unsigned char pinRef)
{
	volatile unsigned char * reg;
	unsigned char pin = pinRef % 8;

	reg = portModeRegister(pinRef / 8);

	if (*reg & (1 << pin))
		return 1;
	else
		return 0;
}



