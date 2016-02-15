#include <hcmmod.h>
#include <usart.h>

#define PS2XIP_Command          0x41
#define PS2XIP_ButtonSet1       0x42
#define PS2XIP_ButtonSet2       0x43
#define PS2XIP_XLeftJoystick    0x44
#define PS2XIP_YLeftJoystick    0x45
#define PS2XIP_XRightJoystick   0x46
#define PS2XIP_YRightJoystick   0x47
#define PS2XIP_beMaster    		0x4D
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


void ps2xip_bemaster(void);
void ps2xip_beslave(void);
void ps2xip_getupdate(unsigned int *, unsigned int*); 
void space(void);
void dot(int);
void dash(int);

