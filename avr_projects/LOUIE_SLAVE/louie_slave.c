#include <ms.h>
#include <twi_slave.c>
#include <delay.c>
#include <avr/interrupt.h>

int main(void)
{
	char louis;

	/* All ports are outputs */
	DDRD = 0xFF;
	DDRC = 0xFF;
	DDRB = 0xFF;

	/* These are the Louie ports */
	/*
	set_bit(PORTB, 0); 		//0
	set_bit(PORTB, 1);		//1 
	set_bit(PORTB, 2);		//2
	set_bit(PORTB, 3);		//3
	set_bit(PORTB, 4);		//4
	set_bit(PORTB, 5);		//5
	set_bit(PORTC, 0);		//6
	set_bit(PORTC, 1);		//7
	set_bit(PORTD, 2);		//8
	set_bit(PORTD, 0);		//9
	*/
	
	twiSlaveInit(0x16);				
	sei();

	while (TRUE) {
		if (twiDataInBuffer) {
			switch (twiData[0]) {
				/* Drive all Louies */
				case 0:
					PORTB = 0x3F;
					PORTC = 0x03;
					PORTD = 0x05;
				break;
				/* Stop all Louies */
				case 1:
					PORTB = 0x00;
					PORTC = 0x00;
					PORTD = 0x00;
				break;
				/* Drive single Louie */
				case 2:
					louis = twiData[1];
					if (louis < 6)
						set_bit(PORTB, louis);
					else if (louis == 6)
						set_bit(PORTC, 0);
					else if (louis == 7)
						set_bit(PORTC, 1);
					else if (louis == 8)
						set_bit(PORTD, 2);
					else if (louis == 9)
						set_bit(PORTD, 0);
				break;
				/* Stop single Louie */
				case 3:
					louis = twiData[1];
					if (louis < 6)
						clear_bit(PORTB, louis);
					else if (louis == 6)
						clear_bit(PORTC, 0);
					else if (louis == 7)
						clear_bit(PORTC, 1);
					else if (louis == 8)
						clear_bit(PORTD, 2);
					else if (louis == 9)
						clear_bit(PORTD, 0);
				break;
			}
			TWCR = TWISLAVEACTIVEACK;
			twiDataInBuffer = FALSE;
		}
	}
}

