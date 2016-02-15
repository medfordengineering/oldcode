/* -------------------------------------------------------------
 Machine Science API
 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   17 March 2007
-----------------------------------------------------------------
============================================================== */
#ifndef _TIMERS_H_
#define _TIMERS_H_

#include <ms.h>

#define CLOCK_STOP		0

#define TMR0_PRE1		1
#define TMR0_PRE8		2
#define TMR0_PRE64		3
#define TMR0_PRE256		4
#define TMR0_PRE1024	5

#define TMR1_PRE1		1
#define TMR1_PRE8		2
#define TMR1_PRE64		3
#define TMR1_PRE256		4
#define TMR1_PRE1024	5

#define TMR2_PRE1		1
#define TMR2_PRE8		2
#define TMR2_PRE32		3
#define TMR2_PRE64		4
#define TMR2_PRE128		5
#define TMR2_PRE256		6
#define TMR2_PRE1024	7

#define TMR0_NORM		0
#define TMR0_PWM_PCX	1
#define TMR0_CTC		2
#define TMR0_PWM_FASTX	3
#define TMR0_PWM_PCO	5
#define TMR0_PWM_FASTO	7

#define TMR2_NORM		0
#define TMR2_PWM_PCX	1
#define TMR2_CTC		2
#define TMR2_PWM_FASTX	3
#define TMR2_PWM_PCO	5
#define TMR2_PWM_FASTO	7

#define TMR_MODE0		0
#define TMR_MODE1		1
#define TMR_MODE2		2
#define TMR_MODE3		3
#define TMR_MODE4		4
#define TMR_MODE5		5
#define TMR_MODE6		6
#define TMR_MODE7		7
#define TMR_MODE8		8
#define TMR_MODE9		9
#define TMR_MODE10		10
#define TMR_MODE11		11
#define TMR_MODE12		12
#define TMR_MODE13		13
#define TMR_MODE14		14
#define TMR_MODE15		15

#define TMR_NORM		0
#define TMR_TOG			1
#define TMR_CLR			2
#define TMR_SET			3

//#define TMR_OVERFLOW	1
//#define TMR_MATCHA		2
//#define TMR_MATCHB		4

#define TMR0_OVERFLOW 		0
#define TMR0_MATCHA 		1
#define TMR0_MATCHB 		2

#define TMR1_OVERFLOW 		0	
#define TMR1_MATCHA 		1
#define TMR1_MATCHB 		2

#define TMR2_OVERFLOW 		0	
#define TMR2_MATCHA 		1
#define TMR2_MATCHB 		2

extern void timer0_pwm_init(char, char);
extern void heartbeat_init( unsigned int );
extern void timer0_interval(unsigned long, char );
extern void timer0_mode(char );
extern void timer0_prescale(int );
extern void timer0_output(char , char );
extern void timer0_interrupt(char, char);
extern void timer0_duty(unsigned char, unsigned char);

extern void timer1_interval(unsigned long, char );
extern void timer1_mode(char );
extern void timer1_prescale(int );
extern void timer1_output(char , char );
extern void timer1_interrupt(char, char);

extern void timer2_interval(unsigned long, char );
extern void timer2_mode(char );
extern void timer2_prescale(int );
extern void timer2_output(char , char );
extern void timer2_interrupt(char );
extern void timer2_duty(unsigned char, unsigned char);

#endif
