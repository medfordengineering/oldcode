
/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2003.  All rights reserved

 Version Date:   11 Apr 2007
-----------------------------------------------------------------
============================================================== */
#include "keyscan.h"

/*Notes:
- An 8 X 8 matrix
- Columns are on pins 0 - 3 and set as outputs
- Rows are on pins 4 - 7 and set as inputs
- The formula for key is the result of column 0 and row 4 being the 15 key (lower right) and
column 3 and row 7 being the 0 key (upper left). 
- c = (c - 3) * -1 or abs(c -= 3)
- r = (r - 7) * - 1 or abs(r - = 7)
*/

void keypad_init(void)
{
	/* Set rows (A4..7) as inputs and colums (A0..3) as outputs */
	KEY_DIR = 0x0F;	
								
	/* Turn on pull up resistors for rows (A4..7) */
	KEY_PORT = 0xF0;

	/* Enable pin change interrupt */
	set_bit(PCICR, PCIE0);

	/* Clear pin change 0-7 interrupt flag */
	set_bit(PCIFR, PCIF0); 

	/* Enable interrupts on upper nibble (A4..7) */
	PCMSK0 = 0xF0; 			

	/* Turn of USART to free D0 and D1 */
	USCSR0B = 0x00;
}

char scan_keypad(void)
{
	char keypress = FALSE;
	signed char c;
	signed char r;

	for(c = 0; c <= 3; c++){

		/* Set all columns high */
		KEY_PORT = 0xFF;

		/* Clear test column low */
		clear_bit(KEY_PORT, c);

		/* Test each row */
		for(r = 4; r <= 7; r++){

			/* Low row equals pressed key */
			if((test_bit(KEY_PIN, r)) == 0){
				keypad.row = ((r - 7) * -1);
				keypad.col = ((c - 3) * -1);
				keypad.key = (keypad.row * 4) + keypad.col;
				keypad.key = (15 - keypad.key) * -1;
				keypress = TRUE;
			}
		}
	}
	/* These last two need to reset when using pin change interrupts */

	/* Set rows (A4..7) as inputs and colums (A0..3) as outputs */
	KEY_DIR = 0x0F;
									
	/* Turn on pull up resistors for rows (A4..7) */
	KEY_PORT = 0xF0;

	return keypress;
}


