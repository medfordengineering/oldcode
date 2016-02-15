/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   24 April 2006
-----------------------------------------------------------------
============================================================== */

#include "lcd.h"
#include "ms_print.h"

void lcd_decimal(unsigned char line, unsigned int value, char digits)
{
	lcd_instruction(line);
	lcd_print(value, 10, UNSIGNED_ZEROS, digits);
}

void lcd_binary(unsigned char line, unsigned int value, char digits)
{
	lcd_instruction(line);
	lcd_print(value, 2, UNSIGNED_ZEROS, digits);
}

void lcd_hexidecimal(unsigned char line, unsigned int value, char digits)
{
	lcd_instruction(line);
	lcd_print(value, 16, UNSIGNED_ZEROS, digits);
}

void lcd_signed(unsigned char line, signed int value, char digits)
{
	lcd_instruction(line);
	lcd_print(value, 10, SIGNED_ZEROS, digits);
}

void lcd_print(signed int value, char base, char zeros, char digits)
{
	char s[MAX_DIGITS];
	ms_sprintf(value, base, zeros, digits, s);
	lcd_string(s);
}


