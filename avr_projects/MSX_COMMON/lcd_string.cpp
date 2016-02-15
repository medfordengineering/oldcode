/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   24 April 2006
-----------------------------------------------------------------
============================================================== */

#include "ms_lcd.h"

void lcd_string(char *s)
{
	while(*s) {
		lcd_byte(*s);
		s++;
	}
}

void lcd_text(unsigned char line, const char * s)
{
	lcd_instruction(line);
 	while(*s) {
  		lcd_byte(*s);
		s++;
	}
}

void lcd_character(unsigned char line, unsigned char c)
{
	lcd_instruction(line);
	lcd_byte(c);
}
