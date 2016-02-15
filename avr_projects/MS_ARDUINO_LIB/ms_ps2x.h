#ifndef _ms_ps2x_h
#define _ms_ps2x_h

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <ms_delay.h>
#include <avr/interrupt.h>
#include <machinescience.h>

#define CTRL_CLK        4
#define CTRL_BYTE_DELAY 3

#define clockCyclesToUS(a) ( ((a) * 1000L) / (FOSC / 1000L) )
#define US_PER_T0_OVERFLOW (clockCyclesToUS(64 * 256))
#define MS_INC (US_PER_T0_OVERFLOW / 1000)
#define FRACTION_INC ((US_PER_T0_OVERFLOW % 1000) >> 3)
#define FRACTION_MAX (1000 >> 3)

//These are our button constants
#define PSB_SELECT      0x0001
#define PSB_L3          0x0002
#define PSB_R3          0x0004
#define PSB_START       0x0008
#define PSB_PAD_UP      0x0010
#define PSB_PAD_RIGHT   0x0020
#define PSB_PAD_DOWN    0x0040
#define PSB_PAD_LEFT    0x0080
#define PSB_L2          0x0100
#define PSB_R2          0x0200
#define PSB_L1          0x0400
#define PSB_R1          0x0800
#define PSB_GREEN       0x1000
#define PSB_RED         0x2000
#define PSB_BLUE        0x4000
#define PSB_PINK        0x8000
#define PSB_TRIANGLE    0x1000
#define PSB_CIRCLE      0x2000
#define PSB_CROSS       0x4000
#define PSB_SQUARE      0x8000

//Guitar  button constants
#define GREEN_FRET		0x0200
#define RED_FRET		0x2000
#define YELLOW_FRET		0x1000
#define BLUE_FRET		0x4000
#define ORANGE_FRET		0x8000
#define STAR_POWER		0x0100
#define UP_STRUM		0x0010
#define DOWN_STRUM		0x0040
#define WHAMMY_BAR		8

//These are stick values
#define PSS_RX 5
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

//These are analog buttons
#define PSAB_PAD_RIGHT   9
#define PSAB_PAD_UP      11
#define PSAB_PAD_DOWN    12
#define PSAB_PAD_LEFT    10
#define PSAB_L2          19
#define PSAB_R2          20
#define PSAB_L1          17
#define PSAB_R1          18
#define PSAB_GREEN       13
#define PSAB_RED         14
#define PSAB_BLUE        15
#define PSAB_PINK        16
#define PSAB_TRIANGLE    13
#define PSAB_CIRCLE      14
#define PSAB_CROSS       15
#define PSAB_SQUARE      16

//These are MS defines and should replace above
#define RIGHT_X 		5
#define RIGHT_Y 		6
#define LEFT_X 			7
#define LEFT_Y 			8

#define BUTTON_L2          19
#define BUTTON_R2          20
#define BUTTON_L1          17
#define BUTTON_R1          18

//Arduino
/*
#define MSCLK	5
#define MSCMD	3
#define MSATT	2
#define MSDAT	4
*/

//Bread
#define MSCLK	1
#define MSCMD	6
#define MSATT	0
#define MSDAT	7

extern unsigned long ms_count(void);
extern void ps2x_init(void);
extern unsigned char NewButtonState(unsigned int);
extern unsigned char NewButtonStateN();
extern unsigned char ButtonPressed(unsigned int);
extern unsigned char ButtonReleased(unsigned int);
extern unsigned char Button(uint16_t);
extern unsigned int ButtonDataByte();
extern unsigned char Analog(unsigned char);
extern unsigned char gp_shiftdata(unsigned char );
extern void sendCMD(unsigned char *, unsigned char );
extern unsigned char readType();
extern void reconfig_gamepad();
extern void read_gp(void);
extern unsigned char config_gp(void);

extern unsigned int last_buttons;
extern unsigned int buttons;	
extern unsigned char read_delay;
extern unsigned char controller_type;	
extern unsigned char en_Rumble;
extern unsigned char en_Pressures;
extern unsigned long last_read;

extern unsigned char PS2data[21];


#endif



