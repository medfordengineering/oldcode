/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   16 March 2007
-----------------------------------------------------------------
============================================================== */
#include "lcd.h"

/* Clear LCD display */
void lcd_clear(void)
{	
	clear_bit(LCD_RS_PRT, LCD_RS_PIN);
 	lcd_send_data(CLEAR_DISPLAY);
	delay_ms(2);
}

/* Send instruction to LCD */
void lcd_instruction(unsigned char c)
{
	clear_bit(LCD_RS_PRT, LCD_RS_PIN);
 	lcd_send_data(c);
}

/* Send byte to LCD */
void lcd_byte(unsigned char c)
{
 	set_bit(LCD_RS_PRT, LCD_RS_PIN);
 	lcd_send_data(c);
}

/* Toggle enable pin */
void lcd_strobe(void)
{
	set_bit(LCD_EN_PRT, LCD_EN_PIN);
	clear_bit(LCD_EN_PRT, LCD_EN_PIN);
}

/* One byte transfer to LCD */
void lcd_send_data(unsigned char c)
{
	LCD_DT_PRT &= 0xF0 >> LCD_START_BIT; 		//Send high nibble of DT
	LCD_DT_PRT |= (c & 0xF0) >> (4 - LCD_START_BIT);
	lcd_strobe();
	
	LCD_DT_PRT &= 0xF0 >> LCD_START_BIT; 		//Send low nibble of DT 
	LCD_DT_PRT |= (c & 0x0F) << LCD_START_BIT;
	lcd_strobe();
	
	delay_us(150);					
}

/* initialize the lcd */
void lcd_init(void)
{
	LCD_DT_PRT &= (0xF0 >> LCD_START_BIT); 	// Clear DT nibble
	clear_bit(LCD_RS_PRT,LCD_RS_PIN);       	// Write instructions
	clear_bit(LCD_RW_PRT,LCD_RW_PIN);			// Write to LCD

	output_nib(LCD_DT_DIR, LCD_START_BIT);		// Set DT nibble to output
	output_bit(LCD_EN_DIR, LCD_EN_PIN);		// Set enable to output
	output_bit(LCD_RS_DIR, LCD_RS_PIN);		// Set register select to output
	output_bit(LCD_RW_DIR, LCD_RW_PIN);		// Set read/write to output

	delay_ms(15);        						// Power on delay 

	LCD_DT_PRT |= (3 << LCD_START_BIT);		// Set 8-bit mode by sending a 3 three times
	
	lcd_strobe();
	delay_ms(5);
	
	lcd_strobe();								// Re-set 8-bit mode 
	delay_us(150);
	
	lcd_strobe();								// Re-set 8-bit mode 
	delay_us(150);
	
	clear_bit(LCD_DT_PRT, LCD_START_BIT);		// Set 4-bit mode by sending a 2
	lcd_strobe();
	delay_us(150);								// Check timing

 	clear_bit(LCD_RS_PRT,LCD_RS_PIN);

	lcd_send_data(LCD_SETUP_0);					// Set: 4-bit mode, 2 lines, 5x8 font 
	lcd_send_data(DISPLAY_OFF);					// Turn display off 
	lcd_send_data(DISPLAY_ON);					// Turn display on
	lcd_send_data(LCD_MODE_0);					// Set: increment cursor, no display shift 
	lcd_send_data(CLEAR_DISPLAY);

	delay_ms(2);
}
