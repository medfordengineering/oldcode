//#include <PS2XIP.h>
#include <ps2mod.h>

#define PS2XIP_Command          0x41
#define PS2XIP_ButtonSet1       0x42
#define PS2XIP_ButtonSet2       0x43
#define PS2XIP_XLeftJoystick    0x44
#define PS2XIP_YLeftJoystick    0x45
#define PS2XIP_XRightJoystick   0x46
#define PS2XIP_YRightJoystick   0x47
#define PS2XIP_beSlave  		0x53

#define Button_Select    0
#define Button_L3        1
#define Button_R3        2
#define Button_Start     3
#define Button_Up        4
#define Button_Right     5
#define Button_Down      6
#define Button_Left      7
#define Button_L2        8
#define Button_R2        9
#define Button_L1        10
#define Button_R1        11
#define Button_Triangle  12
#define Button_Circle    13
#define Button_Cross     14
#define Button_Square    15

#define Joystick_X_Left  	0
#define Joystick_Y_Left 	1
#define Joystick_X_Right  	2
#define Joystick_Y_Right 	3

#define LEFT_X				Joystick_X_Left
#define LEFT_Y				Joystick_Y_Left
#define RIGHT_X				Joystick_X_Right
#define RIGHT_Y				Joystick_Y_Right

// The following code has been copied from the "PS2XIP.cpp" library 

void ps2xip_beslave(void);
void ps2xip_getupdate(unsigned int *, unsigned char*); 
void space(void);
void dot(int);
void dash(int);

void ps2mod_control(char);
signed char ps2mod_joystick(int);
unsigned char ps2mod_button(int);
int isSlave = 1;

/* Function added by Machine Science */
void ps2mod_control(char state)
{
	if (state == DISABLE)
		ps2xip_beslave();
}

signed char ps2mod_joystick(int stick)
{
	unsigned char joy[4];
	unsigned int btn[16];
	signed int value;

	ps2xip_getupdate(btn, joy);
	
	value = ((unsigned int)joy[stick] * 200)/0xff;
	value -= 100;
	if ((stick == 1) || (stick == 3))
		value *= -1;

	return (signed char) value;
}

unsigned char ps2mod_button(int button)
{
	unsigned char joy[4];
	unsigned int btn[16];

	ps2xip_getupdate(btn, joy);
	
	return btn[button];
}

void
ps2xip_beslave()
{
    while (twiBusy());
		delay_ms(10);
		twiMsgSize = 0;
		twiData[0] = ((PS2XIP_MODULE) | (TWI_READ));
		TWCR = TWIMASTERSTART;

}

/*
void
ps2xip_beslave()
{
    while (twiBusy());
    delay_ms(10);
    twiMsgSize = 1;
    twiData[0] = ((PS2XIP_MODULE) | (TWI_WRITE));
    twiData[1] = 0x41;

    TWCR = TWIMASTERSTART;

    while (twiBusy());
    delay_ms(10);
    twiMsgSize = 1;
    twiData[0] = ((PS2XIP_MODULE) | (TWI_READ));
    TWCR = TWIMASTERSTART;
}
*/

void
ps2xip_getupdate(unsigned int *buttons, unsigned char *joys)
{

    unsigned char   buf1;
    unsigned char   buf2;
    unsigned char   xljoy;
    unsigned char   yljoy;
    unsigned char   xrjoy;
    unsigned char   yrjoy;
    unsigned char   r1 = PS2XIP_ButtonSet1;
    unsigned char   r2 = PS2XIP_ButtonSet2;
    unsigned char   r3 = PS2XIP_XLeftJoystick;
    unsigned char   r4 = PS2XIP_YLeftJoystick;
    unsigned char   r5 = PS2XIP_XRightJoystick;
    unsigned char   r6 = PS2XIP_YRightJoystick;
    while (twiBusy());

    //delay_ms(10);

    twiMsgSize = 1;  // keep this 1
    twiData[0] = ((PS2XIP_MODULE) | (TWI_WRITE));
    twiData[1] = r1;
    twiData[2] = r2;
    twiData[3] = r3;
    twiData[4] = r4;
    twiData[5] = r5;
    twiData[6] = r6;

    TWCR = TWIMASTERSTART;
    while (twiBusy());

    //delay_ms(10);

    twiMsgSize = 7;
    twiData[0] = ((PS2XIP_MODULE) | (TWI_READ));
    TWCR = TWIMASTERSTART;
    while (twiBusy());

    //delay_ms(10);
    buf1 = twiData[1];
    buf2 = twiData[2];
    xljoy = twiData[3];
    yljoy = twiData[4];
    xrjoy = twiData[5];
    yrjoy = twiData[6];

    int             i;
    for (i = 0; i < 8; i++) {
        unsigned char   x = buf1 >> i & 0x01;
        unsigned char   y = buf2 >> i & 0x01;
        x = (x - 1);
        y = (y - 1);
        buttons[i] = x;
        buttons[i + 8] = y;
    }
    int             t;
    for (t = 0; t < 16; t++) {
        if (buttons[t] >= 1) {
            buttons[t] = 1;
        } else {
            buttons[t] = 0;
        }
    }
    //delay_ms(10);

		joys[0] = xljoy;
		joys[1] = yljoy;
		joys[2] = xrjoy;
		joys[3] = yrjoy;

}

int             theLight = 0;
int             powerFactor = 1;

void
space()
{

    delay_ms(500);

}

/*
void
dot(int spacer)
{
    high_pin(PORT_D4);
    delay_ms(250);
    low_pin(PORT_D4);
    if (spacer == 1) {
        space();
    }
}

void
dash(int spacer)
{
    high_pin(PORT_D4);
    delay_ms(750);
    low_pin(PORT_D4);
    if (spacer == 1) {
        space();
    }
}
*/
