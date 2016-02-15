/* -------------------------------------------------------------
 Machine Science API
 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   10 January 2006
 
 Test
-----------------------------------------------------------------
============================================================== */
#ifndef _LCD_H_
#define _LCD_H_

#include <ms_delay.h>
#include <ms_print.h>
#include <mspai_common.h>
#include <avr/io.h>

/* LCD DEFINES */

/* lcd control commands */
#define CLEAR_DISPLAY    	0x01
#define CLEAR				0x01
#define CURSOR_LEFT    		0x10
#define CURSOR_RIGHT   		0x14
#define DISPLAY_LEFT   		0x18
#define DISPLAY_RIGHT   	0x1C
#define FIRST_LINE    		0x80
#define SECOND_LINE   		0xC0
#define GOTO_LINE1			0x80
#define GOTO_LINE2			0xC0

/* lcd setup and mode commands */
#define DISPLAY_OFF    		0x08
#define DISPLAY_ON    		0x0C
#define CURSOR_OFF    		0x0C
#define CURSOR_ON    		0x0E
#define CURSOR_BLINK   		0x0F
#define CURSOR_NOBLINK		0x0C
#define LCD_MODE_0    		0x06	/* Increment cursor, no display shift (default) */
#define LCD_MODE_1    		0x05	/* Decrement cursor, display shift */
#define LCD_MODE_2    		0x04	/* Decrement cursor, no display shift */
#define LCD_MODE_3    		0x07	/* Increment cursor, display shift */
#define LCD_SETUP_0    		0x28

#define input_bit(var, pin)		var &= ~(1<<(unsigned char) pin)
#define output_bit(var, pin)	var |= 1<<(unsigned char) pin

#define output_nib(var, pin)    var |= (0x0F << (unsigned char) pin)
#define input_nib(var, pin)     var &= (0xF0 >> (unsigned char) pin)

/*
// FOR ISENSE
#define LCD_EN_PRT		PORTD
#define LCD_EN_DIR		DDRD
#define LCD_EN_PIN		5

#define LCD_RS_PRT		PORTD
#define LCD_RS_DIR		DDRD
#define LCD_RS_PIN		7

#define LCD_RW_PRT		PORTD
#define LCD_RW_DIR		DDRD
#define LCD_RW_PIN		6

#define LCD_DT_PRT		PORTB
#define LCD_DT_DIR		DDRB
#define LCD_START_BIT	0
*/

// FOR BASIC BREADBOARD
#define LCD_EN_PRT		PORTB
#define LCD_EN_DIR		DDRB
#define LCD_EN_PIN		4

#define LCD_RS_PRT		PORTB
#define LCD_RS_DIR		DDRB
#define LCD_RS_PIN		2

#define LCD_RW_PRT		PORTB
#define LCD_RW_DIR		DDRB
#define LCD_RW_PIN		3

#define LCD_DT_PRT		PORTC
#define LCD_DT_DIR		DDRC
#define LCD_START_BIT	0

/*
// MOD BOARDS
#define LCD_EN_PRT		PORTD
#define LCD_EN_DIR		DDRD
#define LCD_EN_PIN		0

#define LCD_RS_PRT		PORTD
#define LCD_RS_DIR		DDRD
#define LCD_RS_PIN		6

#define LCD_RW_PRT		PORTD
#define LCD_RW_DIR		DDRD
#define LCD_RW_PIN		1

#define LCD_DT_PRT		PORTB
#define LCD_DT_DIR		DDRB
#define LCD_START_BIT	0
*/

extern void lcd_clear(void);
extern void lcd_strobe(void);
extern void lcd_byte(unsigned char);   							
extern void lcd_character(unsigned char, unsigned char);
extern void lcd_instruction(unsigned char);  	
extern void lcd_send_data(unsigned char);		
extern void lcd_text(unsigned char, const char *);			
extern void lcd_string(char *);
extern void lcd_init(void);
extern void lcd_print(signed int value, char base, char zeros, char digits);
extern void lcd_store(char, const char *);
extern void lcd_decimal(unsigned char, unsigned int, char);
extern void lcd_binary(unsigned char, unsigned int, char);
extern void lcd_hexidecimal(unsigned char, unsigned int, char);
extern void lcd_signed(unsigned char, signed int, char);
extern void lcd_retrieve(unsigned char);

#endif


