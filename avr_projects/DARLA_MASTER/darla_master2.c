#include <machinescience.h>
#include <timer0.c>
#include <twi_master.c>
#include <twim_io.c>
#include <twim_hcmd.c>
#include <twim_util.c>
#include <twim_mp3.c>
#include <delay.c>

#define RELAY_MODULE	0x22
#define COW_OUT			0x01
#define COW_IN			0x02

/* These numbers corrrespond to the lights on the relays in binary. So the
 * first LED on the second board would be 0x10 or the marquee lights and the main
 * montion should be the last bit or LED on the first board.
 */

#define CL		0x01		//Center Light
#define FM		0x02		//Face Masks
#define DB		0x04		//Disco Ball
#define MM 		0x08		//Main Motion
#define ML 		0x10		//Marquee Lights
#define MK 		0x20 		//Monkeys
#define LM 		0x40		//Lights Main (THESE ARE INVERTED)
#define SL		0x80		//Strobe Lights

/*
unsigned int darlaSet1[10] = {	MK+FM+LM, 950, 
								FM+MM+CL+SL+ML+LM, 200, 
								FM+CL+SL+ML+LM, 1200, 
								FM+CL+SL+ML+DB+LM, 1000};

unsigned int darlaSet2[10] = {	FM+CL+DB+MM+SL, 200, 
								FM+CL+DB+SL, 1200, 
								0x00, 3000};


*/
unsigned int darlaSet1[10] = {	MK+FM+LM, 950, 
								MK+FM+LM+MM,20,
								FM+MM+CL+SL+ML+LM, 200, 
								FM+CL+SL+ML+LM, 1200, 
								FM+CL+SL+ML+DB+LM, 1000};

unsigned int darlaSet2[10] = {	MM, 20,
								FM+CL+DB+MM, 200, 
								FM+CL+DB+SL, 1200, 
								0x00, 3000};



/* Sends single direct relay command. Example, MM turns on main motion relay */
void darla_relay(unsigned int relayAddress)
{
	while(twiBusy());
	twiMsgSize = 3;
	twiData[0] = ((RELAY_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 0;
	twiData[2] = relayAddress;
	twiData[3] = relayAddress >> 8;
	TWCR = TWIMASTERSTART;
}

void main_motion_test(void)
{
	darla_relay(0x08);
	delay_ms(800);
	darla_relay(0x00);
}

void secondary_motion_test (char direction)
{
	if (direction == COW_OUT)
		hcmd_drive_limit(MOTORB, +40, 1);
	else if (direction == COW_IN)
		hcmd_drive_limit(MOTORB, -40, 2);
}
	
void darla_send_set(unsigned int * set, char length)
{
	char i, j = 3;
	while(twiBusy());

	twiMsgSize = (length * 4) + 2;
	twiData[0] = ((RELAY_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 2;
	twiData[2] = length;
	
	for (i = 0; i < (length * 2); i++) {
		twiData[j++] = set[i] >>  8;
		twiData[j++] = set[i]; 
	}
	TWCR = TWIMASTERSTART;
}

int main(void)
{

	signed char remoteValue = 0;
	unsigned char relayValue = 0;
	twiMasterInit(100000);
	sei();
	iomod_text(FIRST_LINE, "Darla Rules");

	while (TRUE) {
		
		remoteValue = iomod_remote();	
		if (remoteValue != -1) {
			iomod_decimal(SECOND_LINE, remoteValue, 4);

			/* This is a good debug routine that demonstrates the problem with
			 * some actions not triggering when there are too many other relays
			 * enabled for example strobe light and main motion.
			 */
			if (remoteValue < 9) {
				relayValue |= (1 << (remoteValue - 1));
				darla_relay(relayValue);
			}
			else {
				relayValue = 0;
				darla_relay(relayValue);
			}
		/*		
			if (remoteValue == 4)
				main_motion_test();
			else if  (remoteValue == 1) {
				darla_send_set(darlaSet1, 5);
				delay_ms(2000);
				secondary_motion_test(COW_OUT);
			}
			else if  (remoteValue == 2) {
				darla_send_set(darlaSet2, 4);
				delay_ms(2000);
				secondary_motion_test(COW_IN);

			} else if  (remoteValue == 7) 
				secondary_motion_test(COW_OUT);
			else if  (remoteValue == 9) 
				secondary_motion_test(COW_IN);
			else
				darla_relay(1 << (remoteValue - 1));
		*/
			
		}
	}
}
