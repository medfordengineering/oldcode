#include <machinescience.h>
#include <iomod.h>
#include <mp3mod.h>

#define LOUIE_MODULE 	0x16
#define FLASH_MODULE 	0x17

volatile unsigned int flashCount;
volatile unsigned int louieCount;

void louie_all_on(void)
{
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((LOUIE_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 0;
	TWCR = TWIMASTERSTART;
}

void louie_all_off(void)
{
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((LOUIE_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 1;
	TWCR = TWIMASTERSTART;
}

void louie_one_on(char louie)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LOUIE_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 2;
	twiData[2] = louie;
	TWCR = TWIMASTERSTART;
}

void louie_one_off(char louie)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((LOUIE_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 3;
	twiData[2] = louie;
	TWCR = TWIMASTERSTART;
}

void flash_all_on(void)
{
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((FLASH_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 0;
	TWCR = TWIMASTERSTART;
}

void flash_all_off(void)
{
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((FLASH_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 1;
	TWCR = TWIMASTERSTART;
}

void flash_one_on(char flash)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((FLASH_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 2;
	twiData[2] = flash;
	TWCR = TWIMASTERSTART;
}


	
void flash_one_off(char flash)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((FLASH_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 3;
	twiData[2] = flash;
	TWCR = TWIMASTERSTART;
}

void louie_start()
{
	louie_all_off();
	flash_all_off();
	mp3mod_play_file("dolly.mp3");
	TIMSK0 = (1<<OCIE0A); 
	//This starts the interrupt and the louie counter
	//note that there is delay between when the mp3
	//starts and the music plays
	louie_all_on();
}

//The flash count is .15 increments. The louieCount is
//in .01 increments. 
ISR (TIMER0_COMPA_vect)
{
	if ((flashCount++) == 15) {
		toggle_bit(PORTD, 2); 
		//This controls the flash rate
		flashCount = 0;
	}

	louieCount++;
}

int main(void)
{
	signed char remoteValue = 0;
	char x;

	/* Set flash pin to output */
	set_bit(DDRD, 2);

	/* Set up Timer0 for ~.01 second interrupt */
	TCCR0A = (2<<WGM00);
	TCCR0B = (5<<CS00);
	//TIMSK0 = (1<<OCIE0A);
	OCR0A = 195;	
	sei();

	network_control(ENABLE); //Enable network control

	iomod_text(FIRST_LINE, "Louie Demo");
	while (remoteValue != 5) {
		remoteValue = iomod_remote();
		delay_ms(100);
		if (remoteValue != -1) {
			iomod_decimal(FIRST_LINE, remoteValue, 4);
		}
		iomod_decimal(SECOND_LINE, x++, 4);
	}
	iomod_text(SECOND_LINE, "Start!");

	louie_start();
	
	while (TRUE) {

		/*
		remoteValue = iomod_remote();

		if (remoteValue != -1) {
			if (remoteValue == 5) {
				louieCount = 0;
				louie_start();
			}
		}
		*/
		if(louieCount == 300) {
			louie_all_on();
		}

		if (louieCount == 505) {
			flash_one_on(6);
			flash_one_on(8);
			flash_one_on(9);
		}
		if (louieCount == 790) {
			flash_all_off();
			flash_one_on(0);
			flash_one_on(3);
			flash_one_on(7);
		}
		else if (louieCount == 1065) {
			flash_all_off();
			flash_one_on(1);
			flash_one_on(4);
			flash_one_on(5);
		}
		else if (louieCount == 1330) {
			flash_all_off();
		}
		else if (louieCount == 1650) {
			flash_all_off();
			flash_one_on(6);
			flash_one_on(8);
			flash_one_on(9);
		}
		else if (louieCount == 2260) {
			flash_all_off();
			flash_one_on(1);
			flash_one_on(3);
			flash_one_on(5);
			flash_one_on(7);
		}
		else if (louieCount == 2900) {
			flash_all_off();
			flash_one_on(6);
			flash_one_on(8);
			flash_one_on(9);
		}
		else if (louieCount == 3490) {
			flash_all_off();
			flash_one_on(1);
			flash_one_on(4);
			flash_one_on(5);
		}
		else if (louieCount == 3660) {
			flash_all_off();
			flash_one_on(0);
			flash_one_on(3);
			flash_one_on(7);
		}
		else if (louieCount == 3870) {
			flash_all_off();
			flash_one_on(1);
			flash_one_on(2);
			flash_one_on(4);
			flash_one_on(5);
		}
		else if (louieCount == 4100) {
			flash_all_off();
			flash_one_on(6);
			flash_one_on(8);
			flash_one_on(9);
		}
		else if (louieCount == 4400) {
			flash_all_off();
			flash_one_on(3);
			flash_one_on(5);
			flash_one_on(6);
			flash_one_on(9);
		}
		else if (louieCount == 4710) {
			flash_all_off();
			flash_one_on(0);
			flash_one_on(1);
			flash_one_on(2);
			flash_one_on(7);
		}
		else if (louieCount == 5380) {
			flash_all_off();
			flash_one_on(6);
			flash_one_on(8);
			flash_one_on(9);
		}
		else if (louieCount == 5640) {
			flash_all_off();
			flash_one_on(0);
			flash_one_on(1);
			flash_one_on(2);
			flash_one_on(7);
		}
		else if (louieCount == 5950) {
			flash_all_off();
			flash_one_on(6);
			flash_one_on(8);
			flash_one_on(9);
		}
		else if (louieCount == 6130) {
			flash_all_off();
			flash_one_on(3);
			flash_one_on(5);
			flash_one_on(6);
			flash_one_on(9);
		}
		else if (louieCount == 6260) {
			flash_all_off();
			flash_one_on(3);
			flash_one_on(5);
			flash_one_on(6);
			flash_one_on(9);
		}
		else if (louieCount == 6440) {
			flash_all_off();
			louie_all_off();
			while(1);
		}
	}
}
	
