/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Fri Sep 25 12:52:41 EDT 2009
-----------------------------------------------------------------
============================================================== */

#ifndef _LINESENSE_H_
#define _LINESENSE_H_

#include <twi_masterslave.h>

/* Commands for ultra sensor */
#define CMD_LINE_SETTWIADR 			0x00
#define CMD_LINE_DISPLAYTWIADR		0x01
#define CMD_LINE_WDSTATE			0x02
#define CMD_LINE_READRANGE			0x03	
#define CMD_LINE_SETHIGHPASS		0x04
#define CMD_LINE_READHIGHPASS		0x05
#define CMD_LINE_SETBAND			0x06
#define CMD_LINE_READBAND			0x07
#define CMD_LINE_SETSTATE			0x08
#define CMD_LINE_READPASS			0x09
#define CMD_LINE_SETCENTER			0x0A
#define CMD_LINE_READCENTER			0x0B
#define CMD_LINE_SETCAPTURE			0x0C
#define CMD_LINE_READCAPTURE		0x0E

extern void set_lineaddress(char , char );
extern void display_lineaddress(char , char );
extern void set_highpass(char , char );
extern unsigned char read_highpass(char );
extern void set_band(char , char );
extern unsigned char read_band(char );
extern void set_state(char , char );
extern unsigned char read_pass(char );
extern void set_center(char, char);
extern unsigned char read_center(char);
extern unsigned int read_line_count(char );
extern void set_line_count(char, char );

#endif
