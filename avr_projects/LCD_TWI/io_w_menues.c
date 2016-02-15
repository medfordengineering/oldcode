#include <machinescience.h>
#include <lcd_initialize.c>
#include <lcd_string.c>
#include <lcd_character.c>
#include <lcd_instruction.c>
//#include <lcd_print.c>
#include <ms_print.c>
#include <twi_slave.c>
#include <sony_remote.c>
#include <timer0.c>
#include <timer1.c>
#include <timer2.c>
#include <delay.c>
#include <twim.h>
#include <pins.h>
#include <clock.c>
#include <calibrate2.c>

/* REMOTE Conversion Code
 * REMOTE KEY		CODE	CONVERSION
 * 0				145		0
 * 1				1		1
 * 2				129		2
 * 3				65		3
 * 4				193		4
 * 5				33		5
 * 6				161		6
 * 7				97		7
 * 8				225		8
 * 9				17		9
 * ON/OFF			169		10
 * CH+				9		11
 * CH-				137		12
 * VOL+				73		13
 * VOL-				201		14
 * OK				209		15	
 * PLAY				89		16
 * STOP				25		17
 * FWD				57		18
 * REV				217		19
 * PSE				153		20
 * REC				185		21
 */

#define MAX_IRCOUNT				22	
#define MENU_LENGTH				20

// 24414 represents timer2 count value * FOSC / timer2 clock (100 * 8M) / 32768
#define OSCCAL_TARGET			24414 //(100 * FOSC) / 32768
// 400 represents .25% accuracy
#define OSCCAL_UPPER_LIMIT		OSCCAL_TARGET + (OSCCAL_TARGET / 400) 
#define OSCCAL_LOWER_LIMIT		OSCCAL_TARGET - (OSCCAL_TARGET / 400)

extern void driveLed(char, char);
extern void driveSpeaker(unsigned int);

const unsigned char remoteCodes[22] = 	{ 	145, 1, 129, 65, 193, 33, 161, 97, 225, 17, 169, 9, 
											137, 73, 201, 209, 89, 25, 57, 217, 153, 185
										};

unsigned int noteFrequency[10];
unsigned int noteDuration[10];
unsigned int noteIndex, noteCounter;
char notesTotal;

//char menuList[6][MENU_LENGTH];
//
char buttonSet[4] = {PORT_D1,PORT_B5,PORT_D0, PORT_D2};
char ledSet[4] = {PORT_D7, PORT_D5, PORT_B1, PORT_D4};
// SHOULD BE UNSIGNED EXPLICIT
char speakerCount;
// SHOULDNT THIS ALSO BE USED TO SET THE PRESCALE
char speakerPrescale = 64;
unsigned int onemsCounter;

ISR (TIMER0_COMPA_vect) 
{
	OCR0A = TCNT0 + speakerCount;
}

ISR (TIMER0_COMPB_vect)
{
	if (onemsCounter++ > toneDuration[songCounter]) {
		if (songCounter++ == numberOfNotes) {
			driveSpeaker(0);
			timer0_interrupt2(TMR0_INTERRUPT_MATCHB, OFF);
			}
		driveSpeaker(toneFrequency[songCounter]);
		onemsCounter = 0;
	}
	OCR0B = TCNT0 + 250;
}

void backlightInit(void)
{
	set_bit(DDRD, 3);
	set_bit(PORTD, 3);
}

/* NOTE: timer0_interrupt2 turns on and off the interrupt on match but does 
 * not disable the output compare. output compare will still run even with
 * the interrupt match off, and must be disabled separately.
 */

void driveSpeaker(unsigned int frequency)
{
	if (frequency) {
		timer0_interrupt2(TMR0_INTERRUPT_MATCHA, ON);
		timer0_output(TMR_TOG, 'A');
		speakerCount = (8000000L / ((long)frequency * 2 * (long)speakerPrescale));
	}
	else {
		timer0_interrupt2(TMR0_INTERRUPT_MATCHA, OFF);
		timer0_output(TMR_NORM, 'A');
	}
}

void speakerInit(void)
{
	output_bit(DDRD, 6);							
	timer0_output(TMR_TOG, 'A');
	timer0_interrupt2(TMR0_INTERRUPT_MATCHA, ON);
	timer0_prescale(TMR0_PRE64); 
	driveSpeaker(OFF); 
}

void onemsCounterInit(char state)
{
	timer0_interrupt2(TMR0_INTERRUPT_MATCHB, state);
	OCR0B = 250;
}

void ledPortInit(void)
{
	int i;
	for (i = 0; i < 4; i++) {
		output_pin(ledSet[i]);
		high_pin(ledSet[i]);
	}
}

void buttonPortInit(void)
{
	int i;
	for (i = 0; i < 4; i++) {
		input_pin(buttonSet[i]);
		pullup_on(buttonSet[i]);
	}
}

void driveLed(char command, char led)
{
	switch (command) {
		case ON:
			low_pin(ledSet[led]);
			break;
		case OFF:
			high_pin(ledSet[led]);
			break;
		case TOGGLE:
			toggle_pin(ledSet[led]);
			break;
	}
}

signed char convertRemote(char code)
{
	signed char i = 0;
	while ((code != remoteCodes[i++]) && (i <= MAX_IRCOUNT)) ;
	if (i > MAX_IRCOUNT) i = 0;
	return (i - 1);
}

/* NOTE: The last two lines are intended to clear garbage characters from the screen.
 * This uses a default of 8 since I don't know where the cursor is at the moment. I need
 * to change this based on the cursor value.
 */

void lcd_print(signed int value, char base, char zeros, char digits)
{
	char s[MAX_DIGITS];
	char i = 8;
	ms_sprintf(value, base, zeros, digits, s);
	lcd_string(s);
	while (i--)
		lcd_character(' ');
}
/*
void displayMenuItem(char menuItem, char startChar)
{
	char i;
	for (i = startChar; i < MENU_LENGTH; i++)
		lcd_character(menuList[menuItem][i]);
}
*/

int main( void )
{
	unsigned int i;
	signed int recValue;
//	signed char menuCount = 0;
//	signed char menuScroll = 0;
	unsigned int calRC;

	twiSlaveInit(0x02);

	lcd_init();

	ledPortInit();
	buttonPortInit();
	speakerInit();
	sony_ir_init();
	backlightInit();
	clock_init();
	onemsCounterInit(ON);

	time_set(12, 30, 1);
	date_set(1, 29, 9);

	sei();
	driveSpeaker(0);
/*
	set_bit(DDRD, 1);
	onemsCounter = 0;
//jj	driveSpeaker(200);
//	speakerDuration = 8000;
	driveSpeaker(toneFrequency[songCounter]);
	while (TRUE);
	while (TRUE) {
		if (onemsCounter > 100) {
	//		toggle_bit(PORTD, 1);
			driveLed(TOGGLE, 0);
			driveLed(TOGGLE, 1);
			driveLed(TOGGLE, 2);
			driveLed(TOGGLE, 3);
			onemsCounter = 0;
		}
	}
*/		

	while(1){

		if (twiDataInBuffer){
			
			switch (twiData[0]){
				case CMD_LCD_TEXT:
					lcd_instruction (twiData[1]);
					i= 1;
					while (++i < twiMsgLength)
						lcd_character (twiData[i]);
					while (i++ < MENU_LENGTH)
						lcd_character(' ');
					break;
					/*
				case CMD_IO_TEXT:
					for (i = 0; i < twiData[1]; i++)
						menuList[twiData[1]] [i] = twiData[i + 2];
					while (i < MENU_LENGTH)
						menuList[twiData[1]] [i++] = ' ';
					lcd_instruction(FIRST_LINE);
					displayMenuItem(0,0);
					break;
					*/
				case CMD_LCD_CHAR:
					lcd_instruction (twiData[1]);
					lcd_character (twiData[2]);
					break;
				case CMD_LCD_CLEAR:
					lcd_instruction (CLEAR_DISPLAY);
					break;
				case CMD_IO_PRINT_SIGNED:
					lcd_instruction (twiData[1]);
					recValue = twiData[2] << 8;
					recValue += twiData[3];
					lcd_print (recValue, BASE_10, SIGNED_ZEROS, twiData[4]);
					break;
				case CMD_LCD_PRINT_DEC:
					lcd_instruction (twiData[1]);
					recValue = twiData[2] << 8;
					recValue += twiData[3];
					lcd_print (recValue, BASE_10, UNSIGNED_ZEROS, twiData[4]);
					break;
				case CMD_LCD_PRINT_BIN:
					lcd_instruction(twiData[1]);
					recValue = (int) (twiData[2] << 8);
					recValue += twiData[3];
					lcd_print(recValue, BASE_2, UNSIGNED_ZEROS, twiData[4]);
					break;
				case CMD_LCD_PRINT_HEX:
					lcd_instruction(twiData[1]);
					recValue = (int) (twiData[2] << 8);
					recValue += twiData[3];
					lcd_print(recValue, BASE_16, UNSIGNED_ZEROS, twiData[4]);
					break;
				case CMD_LCD_BAR:
					lcd_instruction(twiData[1]);
					for (i = 0; i < 16; i++)
						lcd_character (twiData[i + 2]);
					break;
				case CMD_LCD_LED:
					driveLed(twiData[1], twiData[2] - 1);
					break;
				case CMD_LCD_BTN:
					if (pin_value(buttonSet[twiData[1] - 1]))
						twiData[0] = 0;
					else
						twiData[0] = 1;
					break;
				case CMD_LCD_BEEP:
					driveSpeaker((twiData[1] << 8) + twiData[2]);
					break;
				case CMD_IO_TONE:
					//toneCountStart = TCNT2;
					//toneCountTarget = twiData[3];
					//driveSpeaker((twiData[1] << 8) + twiData[2]);
					//toneCountStatus = ON;
					twiData[0] = onemsCounter;
					break;
				case CMD_LCD_REMOTE:
					//if (remoteFlag == TRUE)
						twiData[0] = convertRemote(sony_ir_code);
				//	else
				//		twiData[0] = 55;
				//	remoteFlag = FALSE;
					//sony_ir_code = -4;
					break;
				case CMD_LCD_TIME:
					twiData[0] = time.hours;
					twiData[1] = time.minutes;
					twiData[2] = time.seconds;
					twiData[3] = time.ampm;
					break;
				case CMD_LCD_DATE:
					twiData[0] = date.month;
					twiData[1] = date.day;
					twiData[2] = date.year;
					break;
				case CMD_LCD_SET_TIME:
					time.hours = twiData[1];
					time.minutes = twiData[2];
					time.seconds = twiData[3];
					time.ampm = twiData[4];
					break;
				case CMD_LCD_SET_DATE:
					date.month = twiData[1];
					date.day = twiData[2];
					date.year = twiData[3];
					break;
				case CMD_LCD_BACKLIGHT:
					if (twiData[1] == ON)
						clear_bit(PORTD, 3);
					else if (twiData[1] == OFF)
						set_bit(PORTD, 3);
					else if (twiData[1] == TOGGLE)
						toggle_bit(PORTD, 3);
					break;
					/*
				case CMD_LCD_MENU_ITEM:
					for (i = 0; i < twiData[2]; i++)
						menuList[twiData[1]] [i] = twiData[i + 3];
					while (i < MENU_LENGTH)
						menuList[twiData[1]] [i++] = ' ';
					break;
				case CMD_LCD_SCROLL_VERT:
					// Is there a better way to do this?
					if (twiData[1] == SCROLL_MENU_DOWN) menuCount -= 2; 
					if (menuCount == -1) menuCount = 5;
					if (menuCount == -2) menuCount = 4;
					lcd_instruction(FIRST_LINE);
					displayMenuItem(menuCount++, 0);
					if (menuCount == 6) menuCount = 0;
					lcd_instruction(SECOND_LINE);
					displayMenuItem(menuCount, 0);
					break;
				case CMD_LCD_SCROLL_HORZ:
					lcd_instruction(FIRST_LINE);
					displayMenuItem(menuCount - 1, menuScroll++);
					if (menuScroll == MENU_LENGTH) menuScroll = 0;
					break;
					*/
				case CMD_IO_CALIBRATE_RC:
					calRC = osccalCalibrate(OSCCAL_LOWER_LIMIT, OSCCAL_UPPER_LIMIT);
					twiData[0] = calRC >> 8;
					twiData[1] = calRC;
					break;
			
			}
			twiSlaveActiveACK();
			twiDataInBuffer = FALSE;
		}
	}
}


