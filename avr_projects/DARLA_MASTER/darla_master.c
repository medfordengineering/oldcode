#include <machinescience.h>
#include <timer0.c>
#include <twi_master.c>
#include <twim_io.c>
#include <twim_hcmd.c>
#include <twim_util.c>
#include <twim_mp3.c>
#include <delay.c>

#define RELAY_MODULE	0x22
#define TESTING			0
#define RUNNING			1

/* Gideon: These numbers corrrespond to the lights on the relays in binary. So the
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
#define UC		0x0100		//Up Cow

/* Gideon: Looking at the code on the relay control board, I think the count
 * increments in 4ms or (.004 seconds) so the count below of 950 would coorespond to
 * 950 X .004 3.8 seconds. and the second command set would be on for about 800ms.
 * Note that the second command set and the third only differ by the inclusion of
 * the main motion, (MM) and so I think I designed this so that it would just set
 * and clear the MM motion over 800ms, which should coorespond correctly to a button
 * press, so I am not sure why this would not work. It would be useful to ask Hanny
 * if this makes sense to him, since his control worked for the main motion.
 */

/* Out going motion command sequence */
//unsigned int darlaSet1[10] = {MK+FM+LM, 950, FM+MM+CL+SL+ML+LM, 1400, FM+CL+SL+ML+DB+LM, 3000};
unsigned int darlaSet1[14] = {	MK+FM+LM, 950, 
								FM+DB+MM+CL+SL+ML+LM, 200, 
								FM+DB+CL+SL+ML+LM, 1200, 
								FM+CL+SL+ML+DB+LM, 1800,
								FM+CL+SL+ML+UC, 3000,
								FM+CL+SL+ML, 3000};

/*NOTE: Add discoball to second sequence. add debugging */

/* Gideon: Again note that I did the same thing with the in going motion sequence
 * below. Compare sets one and two.
 */

/* In going motion command sequence */
//unsigned int darlaSet1[10] = {0x01, 400, 0x02, 400, 0x04, 400, 0x08, 400, 0x00, 200};
//unsigned int darlaSet2[10] = {0x10, 400, 0x20, 400, 0x40, 400, 0x80, 400, 0x00, 200};
unsigned int darlaSet2[10] = {	FM+CL+DB+MM+SL, 200, 
								FM+CL+DB+SL+ML, 1200, 
								0x00, 3000};

unsigned int darlaSetT[10] = {	FM+SL+UC, 500,
								DB, 500};

unsigned int masterCounter = 0;

ISR (TIMER0_OVF_vect)
{
	TCNT0 = 78;
	masterCounter++;
}


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

void darla_sequence(unsigned char state)
{
	while(twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((RELAY_MODULE << 1) | (TWI_WRITE));
	twiData[1] = 1;
	twiData[2] = state;
	TWCR = TWIMASTERSTART;
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
	char remotePrev = 0;
	signed char remoteValue;
	unsigned int cowDelay = 0;
	unsigned char cowDirection = FORWARD;
	unsigned char count = 0;
	unsigned int motorb_current = 0;
	char motorb_safety = ON;

	char name[15];

	timer0_prescale(TMR0_PRE1024);
//	set_bit(TIMSK0, TOIE0);

	twiMasterInit(100000);
	sei();
	hcmmod_rc(DISABLE);
	iomod_text(FIRST_LINE, "Darla Rules");
	//darla_relay(0x00);
	
//	while (1);

	while (TRUE) {
		if (masterCounter > cowDelay) {
			clear_bit(TIMSK0, TOIE0);
			masterCounter = 0;
			if (cowDirection == FORWARD)
				hcmd_drive_limit(MOTORB, +40, 1);
			else if (cowDirection == REVERSE)
				hcmd_drive_limit(MOTORB, -40, 2);
		}
		remoteValue = iomod_remote();	
		if (remoteValue != -1) 
			iomod_decimal(FIRST_LINE, remoteValue, 4);
		//if (remoteValue == 1) {
		if ((remoteValue == 1) && (remotePrev != 1)) {
			masterCounter = 0;
			cowDirection = FORWARD;
			cowDelay = 2400;
			set_bit(TIMSK0, TOIE0);
			darla_send_set(darlaSet1, 7);
			mp3mod_play_file("o1NEWMC.mp3");
			/* Need to avoid multiple calls from remote, which seemed to be the
			 * cause of problems with the MP3 not working. */
			//delay_ms(1000);
			remotePrev = 1;
		}
		//else if (remoteValue == 2) {
		else if ((remoteValue == 2) && (remotePrev != 2)) {
			masterCounter = 0;
			cowDirection = REVERSE;
			cowDelay = 50;
			set_bit(TIMSK0, TOIE0);
			darla_send_set(darlaSet2, 3);
			mp3mod_play_file("i2MTC.mp3");
			remotePrev = 2;
			//delay_ms(1000);
		}
		else if (remoteValue == 4)
			hcmd_drive_limit(MOTORB, +40, 1);
		else if (remoteValue == 6)
			hcmd_drive_limit(MOTORB, -40, 2);
		else if (remoteValue == 5)
			hcmd_stop_motor(MOTORB, BRAKE);
		else if (remoteValue == 7) 
			darla_relay(OFF);
		else if (remoteValue == 8)
			darla_relay(MM);
		else if (remoteValue == 9) 
			darla_relay(UC);
	//		darla_send_set(darlaSetT, 2);
		/* Prevent Darla from moving if current draw is too high */
		iomod_decimal(SECOND_LINE, hcmmod_current(MOTORB), 6);
		motorb_current = hcmmod_current(MOTORB);	
		if ((motorb_current > 200) && (motorb_safety == ON))
			hcmd_stop_motor(MOTORB, BRAKE);

	}
}
