/*  --------------------------------------------------------------
 *  Machine Science API
 *
 *  (c) Copyright Machine Science, 2004.  All rights reserved
 *
 *  Version Date:   Wed Nov  5 14:45:24 EST 2008
 *  --------------------------------------------------------------
 *  ============================================================== */

#include "sound.h"

void tone_out(unsigned char port, int frequency, int duration)
{
    long pulse;
    long num;
    long x;
    output_pin( port );
    pulse = 1000000 / frequency;
    pulse = pulse / 2;
    num =  ((long)duration * frequency) / 1000;
    for(x = 0; x <= num; x++)
    {   
        high_pin (port);
        delay_us (pulse);
        low_pin (port);
        delay_us (pulse);
    }
}

