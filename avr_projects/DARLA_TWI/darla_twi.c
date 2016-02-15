#include <machinescience.h>
#include <twi_slave.c>
#include <heartbeat.c>
#include <timer0.c>
#include <delay.c>
//#include <twim.h>

#define SLAVE_ADDRESS		0x22
#define SEQUENCE_LENGTH		12	

unsigned int resolutionCounter, setCounter, setLength;

unsigned int darlaSetOrder[SEQUENCE_LENGTH];
unsigned int darlaSetTime[SEQUENCE_LENGTH];

//unsigned int darlaSetOrder[SEQUENCE_LENGTH] = {	0x0001, 0x0012, 0x0003, 0x0014, 0x0000, 0x0013};
//unsigned int darlaSetTime[SEQUENCE_LENGTH] = {	500,	500, 	500,	500, 	500, 	500};

void load_remote_sequence(void)
{
	char i = 0, j = 2;
	setLength = twiData[1];
	for (i = 0; i < setLength + 1; i++) {
		darlaSetOrder[i] =  (twiData[j] << 8) + twiData[j + 1];
		j += 2;
		darlaSetTime[i] =  (twiData[j] << 8) + twiData[j + 1];
		j += 2;
	}
}

void darla_relays(unsigned int relays)
{
	PORTD = relays & 0x00ff;
	PORTB = (relays >> 8) & 0x000f;
	PORTC = (relays >> 12) & 0x000f;
}

ISR (TIMER0_OVF_vect)
{
	if (resolutionCounter++ > darlaSetTime[setCounter])  {
		darla_relays(darlaSetOrder[++setCounter]); 
		if (setCounter >= setLength - 1) 
			timer0_interrupt(TMR0_OVERFLOW, OFF);
		resolutionCounter = 0;
	}
	TCNT0 = 178;
}

int main(void)
{
	DDRD = 0xff;
	DDRC = 0xff;
	DDRB = 0xff;

	PORTD = 0x00;
	PORTC = 0x00;
	PORTB = 0x00;

	darla_relays(0);
	timer0_prescale(TMR0_PRE1024);
	timer0_interrupt(TMR0_OVERFLOW, OFF);

	twiSlaveInit(SLAVE_ADDRESS);
	
	setCounter = 0;
	resolutionCounter = 0;
	setLength = 0;

	//darla_relays(darlaSetOrder[setCounter]); 
	//sei();
	//while (TRUE) ;
/*
	twiData[0] = 2;
	twiData[1] = 6;

	twiData[2] = 0x00;
	twiData[3] = 0x01; 
	twiData[4] = 0x01;
	twiData[5] = 0xF4;

	twiData[6] = 0x00;
	twiData[7] = 0x02;
	twiData[8] = 0x01;
	twiData[9] = 0xF4;

	twiData[10] = 0x00;
	twiData[11] = 0x04;
	twiData[12] = 0x01;
	twiData[13] = 0x4F;

	twiData[14] = 0x00;
	twiData[15] = 0x03;
	twiData[16] = 0x01;
	twiData[17] = 0x4F;

	twiData[18] = 0x00;
	twiData[19] = 0x05;
	twiData[20] = 0x01;
	twiData[21] = 0x4F;

	twiData[22] = 0x00;
	twiData[23] = 0x00;
	twiData[24] = 0x01;
	twiData[25] = 0x4F;

	twiData[26] = 0x00;
	twiData[27] = 0x06;
	twiData[28] = 0x01;
	twiData[29] = 0x4F;

	load_remote_sequence();
	darla_relays(darlaSetOrder[setCounter]); 
*/
	sei();

//	while (TRUE) ;

	while (TRUE) {
		if (twiDataInBuffer) {
			
			switch (twiData[0]) {
				case 0: //COMMAND
					darla_relays(twiData[1] + (twiData[2] << 8));
				break;
				case 1: //PAUSE
					//darla_relays(OFF);
					timer0_interrupt(TMR0_OVERFLOW, OFF);
				break;
				case 4:	//RETURN
					//darla_relays(RETURN_COW);
				break;
				case 6:	//START
					timer0_interrupt(TMR0_OVERFLOW, ON);
				break;
				case 2: //SEQUENCE
					load_remote_sequence();
					resolutionCounter = 0;
					setCounter = 0;
					timer0_interrupt(TMR0_OVERFLOW, ON);
					darla_relays(darlaSetOrder[0]); 
				break;
				case 5:
					darla_relays(OFF);
					timer0_interrupt(TMR0_OVERFLOW, OFF);
				break;

			}
			//twiSlaveActiveACK();
			TWCR = TWISLAVEACTIVEACK;
			twiDataInBuffer = FALSE;
		}
	}
}
