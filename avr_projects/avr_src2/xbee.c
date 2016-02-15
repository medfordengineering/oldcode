/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date: Sun Jul 19 07:32:16 EDT 2009
-----------------------------------------------------------------
============================================================== */
/*NOTES
*/

#include <xbee.h>

void xb_response(char *rs)
{
	/* Read characters from XBee and check for Return.
	 * All return strings are null terminated. */
	while (TRUE) {
		*rs = usart_buffer_read();
		if (*rs == '\r')
			break;
		rs++;
	}
   *rs = '\0';
}

void xb_enter_command(char *rs) {

	/* Enter XBee command mode. A delay is required * before entering command mode. Returns either OK or
	 * ERROR. */
	delay_ms(XB_COM_DEL);
	usart_text("+++");
	xb_response(rs);
}

void xb_nonvolatile(char *rs)
{
	/* Commit changes to the Xbee to non-volatile memory. */
	usart_text("ATWR");
	usart_write('\r');
	xb_response(rs);
}

void xb_exit_command(char *rs)
{
	/* Leave XBee command mode. */
	usart_text("ATCN");
	usart_write('\r');
	xb_response(rs);
}

//void xb_get_reg(const char *command, char *rs)
void xb_get_reg(char *command, char *rs)
{
	/* Read the value of an XBee register.
	 * Returns a value of the register or 
	 * maybe ERROR. */
	usart_text("AT");
	usart_text(command);
	usart_write('\r');
	xb_response(rs);
}

//void xb_set_reg(const char *command, char *rs)
void xb_set_reg(char *command, char *rs)
{
	/* Write value to an XBee register. 
	 * Returns OK or maybe ERROR. */
	usart_text("AT");
	usart_text(command);
	usart_write('\r');
	xb_response(rs);
}

void xb_wake(void)
{
	clear_bit(PORTD, 7);
}

void xb_sleep(void)
{
	set_bit(PORTD, 7);
}
