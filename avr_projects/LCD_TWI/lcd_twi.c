//#define LARGE_BUFFER

#include <machinescience.h>
#include <iomod.h>
//#include <lcd_initialize.c>
#include <lcd_string.c>
//#include <lcd_character.c>
//#include <lcd_instruction.c>
//#include <lcd_clear.c>
//#include <lcd_text.c>
#include <lcd.c>
#include <lcd_print.c>
#include <ms_print.c>
#include <twi_slave.c>
#include <remote.c>
#include <timer0.c>
#include <timer1.c>
#include <timer2.c>
#include <delay.c>
#include <pins.h>
#include <clock.c>
//#include <calibrate2.c>
#include <calibrate.c>

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

#define CMD_LCD_TEXT			0x21
#define CMD_LCD_CHAR			0x22
#define CMD_LCD_CLEAR			0x25
#define CMD_LCD_PRINT_DEC		0x26
#define CMD_LCD_PRINT_BIN		0x27
#define CMD_LCD_PRINT_HEX		0x28
#define CMD_LCD_BAR				0x29
#define CMD_LCD_LED				0x30
#define CMD_LCD_BTN				0x2B
#define CMD_LCD_BEEP			0x2C
#define CMD_LCD_REMOTE			0x2D
#define CMD_LCD_TIME			0x2E
#define CMD_LCD_DATE			0x2F
#define CMD_LCD_SET_TIME		0x31
#define CMD_LCD_SET_DATE		0x32
#define CMD_LCD_BACKLIGHT		0x33
#define CMD_LCD_MENU_ITEM		0x34
#define CMD_LCD_SCROLL_VERT		0x35
#define CMD_LCD_SCROLL_HORZ		0x36

#define CMD_IO_TEXT				0x00
#define CMD_IO_CURSOR			0x01
#define CMD_IO_CALIBRATE_RC		0x02
#define CMD_IO_PRINT_SIGNED		0x03
#define CMD_IO_TONE				0x04
#define CMD_IO_DISPLAY_TIME		0x05
#define CMD_IO_DISPLAY_DATE		0x06
#define CMD_IO_BTN_ALL			0x07
#define CMD_IO_LED_ALL			0x08

#define MAX_IRCOUNT				22	
#define MENU_LENGTH				20

#define SECONDS					0
#define MINUTES					1
#define HOURS					2
#define AMPM					3
#define MONTH					4
#define DAY						5
#define YEAR					6

// 24414 represents timer2 count value * FOSC / timer2 clock (100 * 8M) / 32768
#define OSCCAL_TARGET			24414 //(100 * FOSC) / 32768
// 400 represents .25% accuracy
#define OSCCAL_UPPER_LIMIT		OSCCAL_TARGET + (OSCCAL_TARGET / 400) 
#define OSCCAL_LOWER_LIMIT		OSCCAL_TARGET - (OSCCAL_TARGET / 400)

static void driveLed(char, char);
static void driveSpeaker(unsigned int);

const unsigned char remoteCodes[22] = 	{ 	145, 1, 129, 65, 193, 33, 161, 97, 225, 17, 169, 9, 
											137, 73, 201, 209, 89, 25, 57, 217, 153, 185
										};

unsigned int noteFrequency[75];
unsigned int noteDuration[75];
//unsigned int noteFrequency[12] = {523, 587, 659, 784, 659, 784, 1047};
//unsigned int noteDuration[12] = {120, 120, 120, 240, 120, 480, 960};
unsigned int noteIndex, noteCounter;
char notesTotal;
unsigned char speakerCount;

char buttonSet[4] = {PORT_D1,PORT_B5,PORT_D0, PORT_D2};
char ledSet[4] = {PORT_D7, PORT_D5, PORT_B1, PORT_D4};

// SHOULDNT THIS ALSO BE USED TO SET THE PRESCALE
char speakerPrescale = 64;

ISR (TIMER0_COMPA_vect) 
{
	OCR0A = TCNT0 + speakerCount;
}

// NOTE THAT THIS DOES NOT PLAY FIRST NOTE. THIS MUST BE PRIMED BELOW. BETTER WAY?
ISR (TIMER0_COMPB_vect)
{
	if (noteCounter++ > noteDuration[noteIndex]) {
		if (noteIndex++ == (notesTotal - 1)) {
			driveSpeaker(0);
			//timer0_interrupt2(TMR0_INTERRUPT_MATCHB, OFF);
			timer0_interrupt(TMR0_MATCHB, OFF);
			}
		else {
			driveSpeaker(noteFrequency[noteIndex]);
			noteCounter = 0;
		}
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
		//timer0_interrupt2(TMR0_INTERRUPT_MATCHA, ON);
		timer0_interrupt(TMR0_MATCHA, ON);
		timer0_output(TMR_TOG, 'A');
		if (frequency > 250) 
			speakerCount = (8000000L / ((long)frequency * 2 * (long)speakerPrescale));
		else 
			speakerCount = 250;
	}
	else {
		//timer0_interrupt2(TMR0_INTERRUPT_MATCHA, OFF);
		timer0_interrupt(TMR0_MATCHA, OFF);
		timer0_output(TMR_NORM, 'A');
	}
}

void speakerInit(void)
{
	output_bit(DDRD, 6);							
	timer0_output(TMR_TOG, 'A');
	//timer0_interrupt2(TMR0_INTERRUPT_MATCHA, ON);
	timer0_interrupt(TMR0_MATCHA, ON);
	timer0_prescale(TMR0_PRE64); 
	driveSpeaker(OFF); 
}

void noteCounterStart(void)
{
	//timer0_interrupt2(TMR0_INTERRUPT_MATCHB, ON);
	timer0_interrupt(TMR0_MATCHB, ON);
	driveSpeaker(noteFrequency[0]);
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

void driveLeds(char leds)
{
	int i;
	for (i = 0; i < 4; i++) {
		high_pin(ledSet[i]);
		if (test_bit(leds, i))
			low_pin(ledSet[i]);
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

void lcd_printf(signed int value, char base, char zeros, char digits)
{
	char s[MAX_DIGITS];
	char i = 8;
	ms_sprintf(value, base, zeros, digits, s);
	lcd_string(s);
	while (i--)
		lcd_byte(' ');
}

void lcd_print_no_blank(signed int value, char base, char zeros, char digits)
{
	char s[MAX_DIGITS];
	ms_sprintf(value, base, zeros, digits, s);
	lcd_string(s);
}

int main( void )
{
	unsigned int i, j;
	signed int recValue;
	unsigned int calRC;

	twiSlaveInit(0x02);

	lcd_init();

	ledPortInit();
	buttonPortInit();
	speakerInit();
	//sony_ir_init();
	remote_init();
	backlightInit();
	clock_init();

	time_set(12, 30, 1);
	date_set(1, 29, 9);

	sei();
	driveSpeaker(0);

	//notesTotal = 7;
	//noteCounterStart();
		
	while(1){

		if (twiDataInBuffer){
			
			switch (twiData[0]){
				case CMD_LCD_TEXT:
					lcd_instruction (twiData[1]);
					i= 1;
					while (++i < twiMsgLength)
						lcd_byte (twiData[i]);
					while (i++ < MENU_LENGTH)
						lcd_byte(' ');
					break;
				case CMD_LCD_CHAR:
					lcd_instruction (twiData[1]);
					lcd_byte (twiData[2]);
					break;
				case CMD_LCD_CLEAR:
					lcd_clear();
					break;
				case CMD_IO_PRINT_SIGNED:
					lcd_instruction (twiData[1]);
					recValue = twiData[2] << 8;
					recValue += twiData[3];
					lcd_printf (recValue, BASE_10, SIGNED_ZEROS, twiData[4]);
					break;
				case CMD_LCD_PRINT_DEC:
					lcd_instruction (twiData[1]);
					recValue = twiData[2] << 8;
					recValue += twiData[3];
					lcd_printf (recValue, BASE_10, UNSIGNED_ZEROS, twiData[4]);
					break;
				case CMD_LCD_PRINT_BIN:
					lcd_instruction(twiData[1]);
					recValue = (int) (twiData[2] << 8);
					recValue += twiData[3];
					lcd_printf(recValue, BASE_2, UNSIGNED_ZEROS, twiData[4]);
					break;
				case CMD_LCD_PRINT_HEX:
					lcd_instruction(twiData[1]);
					recValue = (int) (twiData[2] << 8);
					recValue += twiData[3];
					lcd_printf(recValue, BASE_16, UNSIGNED_ZEROS, twiData[4]);
					break;
				case CMD_LCD_BAR:
					lcd_instruction(twiData[1]);
					for (i = 0; i < 16; i++)
						lcd_byte (twiData[i + 2]);
					break;
				case CMD_LCD_LED:
					driveLed(twiData[1], twiData[2] - 1);
					break;
				case CMD_IO_LED_ALL:
					driveLeds(twiData[1]);
					break;
				case CMD_LCD_BTN:
					if (pin_value(buttonSet[twiData[1] - 1]))
						twiData[0] = 0;
					else
						twiData[0] = 1;
					break;
				case CMD_IO_BTN_ALL:
					j = 0;
					for (i = 0; i < 4; i++) 
						if (pin_value(buttonSet[i]) == LOW) 
							j |= (1 << i);
					twiData[0] = j;
					break;
				case CMD_LCD_BEEP:
					driveSpeaker((twiData[1] << 8) + twiData[2]);
					break;
				case CMD_IO_TONE:
					j = 2;
					notesTotal = twiData[1];
					for (i = 0; i < notesTotal; i++) {	
						noteFrequency[i] = (twiData[j] << 8) + twiData[j + 1];
						j += 2;
						noteDuration[i] =  ((twiData[j] << 8) + twiData[j + 1]) / 2;
						j += 2;
					}
					noteCounterStart();
					break;
				case CMD_LCD_REMOTE:
					twiData[0] = convertRemote(sony_ir_code);
					sony_ir_code = -1;
					break;
					// TIME SHOULD REALLY BE AN ARRAY ON THE SLAVE -- A LOT EASIER 
					// DO THIS WHEN CHANGE TO 2000
				case CMD_LCD_TIME:
					switch (twiData[1]) {
						case SECONDS:
							twiData[0] = time.seconds;
							break;
						case MINUTES:
							twiData[0] = time.minutes;
							break;
						case HOURS:
							twiData[0] = time.hours;
							break;
						case AMPM:
							twiData[0] = time.ampm;
							break;
					}
					break;
				case CMD_LCD_DATE:
					switch (twiData[1]) {
						case MONTH:
							twiData[0] = date.month;
							break;
						case DAY:
							twiData[0] = date.day;
							break;
						case YEAR:
							twiData[0] = date.year;
							break;
					}
					break;
				case CMD_LCD_SET_TIME:
					time.hours = twiData[1];
					time.minutes = twiData[2];
					time.seconds = twiData[3];
					time.ampm = twiData[4];
					break;
				case CMD_IO_SET_TIME:
					if (twiData[1] == 0)
						time.hours = twiData[2];
					if (twiData[1] == 1)
						time.minutes = twiData[2];
					if (twiData[1] == 2)
						time.seconds = twiData[2];
					if (twiData[1] == 3)
						time.ampm = twiData[2];
					break;
				case CMD_LCD_SET_DATE:
					date.month = twiData[1];
					date.day = twiData[2];
					date.year = twiData[3];
					break;
				case CMD_IO_DISPLAY_TIME:
					lcd_instruction(twiData[1]);
					lcd_print_no_blank(time.hours, BASE_10, UNSIGNED_ZEROS, 2);
					lcd_byte(':');
					lcd_print_no_blank(time.minutes, BASE_10, UNSIGNED_ZEROS, 2);
					lcd_byte(':');
					lcd_print_no_blank(time.seconds, BASE_10, UNSIGNED_ZEROS, 2);
					if (time.ampm)
						lcd_text(twiData[1]+8,"PM    ");
					else
						lcd_text(twiData[1]+8,"AM    ");
					break;
				case CMD_IO_DISPLAY_DATE:
					lcd_instruction(twiData[1]);
					lcd_print_no_blank(date.month, BASE_10, UNSIGNED_ZEROS, 2);
					lcd_byte('/');
					lcd_print_no_blank(date.day, BASE_10, UNSIGNED_ZEROS, 2);
					lcd_byte('/');
					lcd_print_no_blank(date.year, BASE_10, UNSIGNED_ZEROS, 2);
					break;
				case CMD_LCD_BACKLIGHT:
					if (twiData[1] == ON)
						clear_bit(PORTD, 3);
					else if (twiData[1] == OFF)
						set_bit(PORTD, 3);
					else if (twiData[1] == TOGGLE)
						toggle_bit(PORTD, 3);
					break;
				case CMD_IO_CALIBRATE_RC:
					calRC = osccalCalibrate(OSCCAL_LOWER_LIMIT, OSCCAL_UPPER_LIMIT);
					twiData[0] = calRC >> 8;
					twiData[1] = calRC;
					break;
			
			}
			TWCR = TWISLAVEACTIVEACK;
			twiDataInBuffer = FALSE;
		}
	}
}


