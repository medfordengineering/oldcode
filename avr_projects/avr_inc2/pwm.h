/* -------------------------------------------------------------
 Machine Science API
 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   Wed Oct 27 15:45:38 EDT 2010
-----------------------------------------------------------------
============================================================== */
#ifndef _PWM_H
#define _PWM_H

#include <avr/io.h>
#include <ms.h>

extern void pwm_init(void);
extern void pwm_duty(char, unsigned char);
extern void pwm_freq(char);

#endif
