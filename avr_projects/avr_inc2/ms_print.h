/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   10 January 2006
-----------------------------------------------------------------
============================================================== */

/* NOTES
a) Need to ifdef header for frequencies different from 4mHz
b) Possible elimination of pulse_start with -=
*/

#ifndef _DIGITS_H_
#define _DIGITS_H_

/* DIGIT DEFINES */

#define LEFT_JUSTIFY	0
#define SIGNED_ZEROS	1
#define RIGHT_JUSTIFY	2
#define UNSIGNED_ZEROS  3

#define MAX_DIGITS		16

#define BASE_10			10
#define BASE_2			2
#define BASE_16			16

extern void ms_reverse(char *, char);
extern void ms_sprintf(signed int, char, char, char, char *);

#endif
