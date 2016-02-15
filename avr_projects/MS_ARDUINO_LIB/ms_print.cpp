/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date: Mon Nov 19 10:57:54 EST 2007
-----------------------------------------------------------------
============================================================== */

#include <ms_print.h>

void ms_reverse(char s[], char slen)
{
	unsigned char i, j, c;
	for(i = 0, j = slen - 1	; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

void ms_sprintf(signed int n, char base, char justify, char digits, char *s)
{
	unsigned char i = 0;
	signed int sign;

	if ((sign = n) < 0)					//Check for negative n and store in sign
		n = -n;							//Absolute value of n

	/* Convert n to string in correct base value */
	do{
		s[i] = n % base + '0';			//Convert n and jump to first ASCII numeral
		if(s[i] > '9') s[i] += 7;		//Jump to A,B.. for base greater than 10
		i++;
	}while ((n /= base) > 0);			//Check for end of value


	/* Reverse string, add zeros and justify */
	switch (justify)
	{
		case SIGNED_ZEROS:
			for(; i < digits; i++)
				s[i] = '0';
			if (sign < 0) 
				s[i++] = '-';
			else
				s[i++] = '+';
			ms_reverse(s, i);
			break;
	
		case UNSIGNED_ZEROS:
			for(; i < digits; i++)
				s[i] = '0';
			ms_reverse(s, i);
			break;
		
		case RIGHT_JUSTIFY:
			if (sign < 0) s[i++] = '-';			
			for(; i < digits; i++)
				s[i] = ' ';
			ms_reverse(s, i);
			break;

		case LEFT_JUSTIFY:
		default:
			if (sign < 0) s[i++] = '-';
			ms_reverse(s, i);
			for(; i < digits; i++)
				s[i] = ' ';
			break;

	}
	s[i] = '\0';
}

