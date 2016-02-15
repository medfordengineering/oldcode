/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   18 Feburary 2006
-----------------------------------------------------------------
============================================================== */

#include <ms_usart.h>


/* sends value coded as decimal to hardware usart */
void usart_decimal(unsigned int value, char digits)
{
	char s[MAX_DIGITS];
	//ms_sprintf(value, BASE_10, LEFT_JUSTIFY, 9, s);
	ms_sprintf(value, BASE_10, UNSIGNED_ZEROS, digits, s);

 	//usart_string(s);
	usart_text(s);
}

/* sends value coded as signed to hardware usart */
void usart_signed(signed int value, char digits)
{
	char s[MAX_DIGITS];
	ms_sprintf(value, BASE_10, SIGNED_ZEROS, digits, s);

 	//usart_string(s);
	usart_text(s);
}

/* sends value coded as binary to hardware usart */
void usart_binary(unsigned int value, char digits)
{
	char s[MAX_DIGITS];
	ms_sprintf(value, BASE_2, UNSIGNED_ZEROS, digits, s);

	usart_text(s);
}

/* sends value coded as hexidecimal to hardware usart */
void usart_hexidecimal(unsigned int value, char digits)
{
	char s[MAX_DIGITS];
	ms_sprintf(value, BASE_16, UNSIGNED_ZEROS, digits, s);

	usart_text(s);
}

/* sends value coded in any base to hardware usart */
void usart_digits(signed int value, char base, char zeros, char digits)
{
	char s[MAX_DIGITS];
	ms_sprintf(value, base, zeros, digits, s);
	//usart_string(s);
	usart_text(s);
}
