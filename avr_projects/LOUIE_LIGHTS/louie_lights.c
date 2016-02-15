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
	set_bit(PORTB, ?); 		//0
	set_bit(PORTD, 0);		//1 
	set_bit(PORTB, ?);		//2
	set_bit(PORTD, 1);		//3
	set_bit(PORTB, ?);		//4
	set_bit(PORTD, 2);		//5
	set_bit(PORTD, 3);		//6
	set_bit(PORTB, ?);		//7
	set_bit(PORTC, 0);		//8
	set_bit(PORTC, 1);		//9
	*/
	
	twiSlaveInit(0x17);				
	sei();

	while (TRUE) {
		if (twiDataInBuffer) {
			switch (twiData[0]) {
				/* Drive all flashers */
				case 0:
					PORTB = 0x0F;
					PORTC = 0x0F;
					PORTD = 0x05;
				break;
				/* Stop all flashers */
				case 1:
					PORTB = 0x00;
					PORTC = 0x00;
					PORTD = 0x00;
				break;
				/* Drive single flasher */
				case 2:
					louis = twiData[1];
					if (louis == 0)
						set_bit(PORTB, 0);
					else if (louis == 1)
						set_bit(PORTD, 0);
					else if (louis == 2)
						set_bit(PORTB, 1);
					else if (louis == 3)
						set_bit(PORTD, 1);
					else if (louis == 4)
						set_bit(PORTB, 2);
					else if (louis == 5)
						set_bit(PORTD, 2);
					else if (louis == 6)
						set_bit(PORTD, 3);
					else if (louis == 7)
						set_bit(PORTB, 3);
					else if (louis == 8)
						set_bit(PORTC, 0);
					else if (louis == 9)
						set_bit(PORTC, 1);
				break;
				/* Stop single flasher */
				case 3:
				louis = twiData[1];
					if (louis == 0)
						clear_bit(PORTB, 0);
					else if (louis == 1)
						clear_bit(PORTD, 0);
					else if (louis == 2)
						clear_bit(PORTB, 1);
					else if (louis == 3)
						clear_bit(PORTD, 1);
					else if (louis == 4)
						clear_bit(PORTB, 2);
					else if (louis == 5)
						clear_bit(PORTD, 2);
					else if (louis == 6)
						clear_bit(PORTD, 3);
					else if (louis == 7)
						clear_bit(PORTB, 3);
					else if (louis == 8)
						clear_bit(PORTC, 0);
					else if (louis == 9)
						clear_bit(PORTC, 1);
				break;
			}
			TWCR = TWISLAVEACTIVEACK;
			twiDataInBuffer = FALSE;
		}
	}
}

