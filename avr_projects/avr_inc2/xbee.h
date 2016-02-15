//#include "machinescience.h"
#include <ms.h>
#include "usart.h"
#include "delay.h"
#include "pins.h"

#ifndef _XBEE_H_
#define _XBEE_H_

//Function prototypes
/*
extern void xbee_string(const char*);
*/

extern void xb_response(char*);
extern void xb_enter_command(char*);
extern void xb_nonvolatile(char*);
extern void xb_exit_command(char*);
//extern void xb_get_reg(const char*, char*);
extern void xb_get_reg(char*, char*);
//extern void xb_set_reg(const char*, char*);
extern void xb_set_reg(char*, char*);
extern void xb_wake(void);
extern void xb_sleep(void);
extern void xb_init(void);

//Global variables and defined constants
//#define XB_COM_DEL 1000
#define XB_COM_DEL 100



#endif
