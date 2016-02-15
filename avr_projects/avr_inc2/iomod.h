/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Fri Sep 25 12:52:41 EDT 2009
-----------------------------------------------------------------
============================================================== */

#ifndef _IOMOD_H_
#define _IOMOD_H_

#ifndef _SLAVE
	#include <twi_master.h>
#endif

#include <string.h>
#include <delay.h>

/* Commands for LCD module */
#define CMD_LCD_TEXT			0x21
#define CMD_LCD_CHAR			0x22
#define CMD_LCD_CLEAR			0x25
#define CMD_LCD_PRINT_DEC		0x26
#define CMD_LCD_PRINT_BIN		0x27
#define CMD_LCD_PRINT_HEX		0x28
#define CMD_LCD_BAR				0x29
#define CMD_LCD_LED				0x30
#define CMD_LCD_BTN				0x2B
#define CMD_LCD_BEEP			0x2C
#define CMD_LCD_REMOTE			0x2D
#define CMD_LCD_TIME			0x2E
#define CMD_LCD_DATE			0x2F
#define CMD_LCD_SET_TIME		0x31
#define CMD_LCD_SET_DATE		0x32
#define CMD_LCD_BACKLIGHT		0x33
#define CMD_LCD_MENU_ITEM		0x34
#define CMD_LCD_SCROLL_VERT		0x35
#define CMD_LCD_SCROLL_HORZ		0x36

#define CMD_IO_TEXT				0x00
#define CMD_IO_CURSOR			0x01
#define CMD_IO_CALIBRATE_RC		0x02
#define CMD_IO_PRINT_SIGNED		0x03
#define CMD_IO_TONE				0x04
#define CMD_IO_DISPLAY_TIME		0x05
#define CMD_IO_DISPLAY_DATE		0x06
#define CMD_IO_BTN_ALL			0x07
#define CMD_IO_LED_ALL			0x08
#define CMD_IO_SET_TIME			0x09
#define CMD_IO_SET_DATE			0x0A

/* Notes */
//#define A3						220
//#define A3S						233
//#define B3F						233
//#define B3						247
#define C4						262
#define C4S						277
#define D4F						277
#define D4						294
#define D4S						311
#define E4F						311
#define E4						330
#define F4						349
#define F4S						370
#define G4F						370
#define G4						392
#define G4S						415
#define A4F						415
#define A4						440
#define A4S						466
#define B4F						466
#define B4						494
#define C5						523
#define C5S						554
#define D5F						554
#define D5						587
#define D5S						622
#define E5F						622
#define E5						659
#define F5						698
#define F5S						740
#define G5F						740
#define G5						784
#define G5S						830
#define A5F						830
#define A5						880
#define A5S						932
#define B5F 					932
#define B5						988
#define C6						1047
#define C6S						1109
#define D6F						1109
#define D6						1175
#define D6S						1245
#define E6F						1245
#define E6						1319
#define F6						1397
#define F6S						1480
#define G6F						1480
#define G6						1568
#define G6S						1661
#define A6F						1661
#define A6						1760
#define A6S						1865
#define B6F						1865
#define B6						1975

#define No64					25	
#define No32					No64 * 2
#define Nd32					No64 * 3
#define No16					No64 * 4
#define Nd16					No64 * 6
#define No08					No64 * 8
#define Nd08					No64 * 12 
#define No04					No64 * 16
#define Nd04					No64 * 24
#define No02					No64 * 32
#define Nd02					No64 * 48
#define No01					No64 * 64 
#define Nd01					No64 * 96

#define SECONDS					0
#define MINUTES					1
#define HOURS					2
#define AMPM					3
#define MONTH					4
#define DAY						5
#define YEAR					6

#define	BTN1					0x01	
#define BTN2					0x02
#define BTN3					0x04
#define BTN4					0x08
#define	LED1					0x01	
#define LED2					0x02
#define LED3					0x04
#define LED4					0x08

#define FIRST_LINE    			0x80
#define SECOND_LINE   			0xC0

#define PRESSED		1
#define UNPRESSED	0

/* LCD module functions */
extern void iomod_character( unsigned char , unsigned char );
extern void iomod_text( unsigned char , char * );
extern void iomod_decimal( unsigned char , unsigned int , unsigned char );
extern void iomod_hexadecimal( unsigned char , unsigned int , unsigned char );
extern void iomod_binary (unsigned char , unsigned int , unsigned char );
extern void iomod_bar (unsigned char , unsigned char );
extern void iomod_clear (void);
extern signed char iomod_remote(void);
extern void iomod_beep(unsigned int);
extern void iomod_song(unsigned int *, unsigned char);
extern void iomod_led( unsigned char, unsigned char);
extern void iomod_leds( unsigned char );
extern unsigned char iomod_button(unsigned char);
extern unsigned char iomod_buttons(void);
extern unsigned char iomod_time(unsigned char);
extern unsigned char iomod_date(unsigned char);
extern void iomod_set_time	(	unsigned char,
								unsigned char,
								unsigned char,
								unsigned char
							);
extern void iomod_set_date	(	unsigned char,
								unsigned char,
								unsigned char
							);
extern void iomod_display_time(unsigned char);
extern void iomod_display_date(unsigned char);
extern void iomod_backlight(unsigned char);

extern unsigned int iomod_calibrate_system_clock(void);
extern void iomod_signed( unsigned char , unsigned int , unsigned char );

#endif
