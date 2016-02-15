/* BUG: I need to reassign motor_module to motor_modulea, etc. */

/* Addresses for all the modules */
/* Consider adding one to the upper nibble to avoid restricted addresses */
#define GENERAL_CALL			0x00
//#define RESERVED				0x01
#define LCD_MODULE				0x02
#define KEYPAD_MODULE			0x03
#define MOTOR_MODULE			0x04	
#define MOTOR_MODULEA			0x05	
#define MOTOR_MODULEB			0x06	
#define MOTOR_MODULEC			0x07	
#define PINPOINT_MODULE			0x08
#define MP3_MODULE				0x09
#define XBEE_MODULE				0x0A
#define SENSOR_MODULE			0x0B
#define PS2XIP_MODULE			0x12
#define COMPASS_MODULE			0x21

