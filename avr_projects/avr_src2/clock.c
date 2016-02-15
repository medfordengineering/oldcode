/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date: Fri Nov 30 12:22:45 EST 2007
-----------------------------------------------------------------
============================================================== */

#include "clock.h"

void date_set( char month, char day, char year )
{
	date.day = day;
	date.month = month;
	date.year = year;
}

void time_set( char hours, char minutes, char ampm )
{
	time.hours = hours;
	time.minutes = minutes;
	time.ampm = ampm;
}

void clock_init( void )
{
    /* Enable asynchonous mode for timer2 */
    set_bit(ASSR, AS2);

    /* Enable 128 prescale for timer2 (1 second update with 32.768 crystal) */
    set_bit(TCCR2B, CS20);
    set_bit(TCCR2B, CS22);	

    /* Enable timer2 interrupt on overflow */
    set_bit(TIMSK2, TOIE2);

    /* Clear timer2 overflow flag */
    set_bit(TIFR2, TOV2);

}

SIGNAL (SIG_OVERFLOW2)
{

    time.seconds++;
    
    if(time.seconds == 60){
        time.seconds = 0;
        time.minutes++;
    }
    if(time.minutes == 60){
        time.minutes = 0;
        time.hours++;
    }
	if(time.hours == 13){
		time.hours = 1;	
	}
	if (	(time.hours == 12) && 
			(time.minutes == 0) &&
			(time.seconds == 0) 
		)
		toggle_bit(time.ampm, 0);

	if (	(time.hours == 12) && 
			(time.minutes == 0) &&
			(time.seconds == 0) &&
			(time.ampm == TIME_AM)
		)
		date.day++;

	if(date.day == (daysOfMonths[date.month - 1] + 1)){
		date.month++;
		date.day = 1;
	}
	if(date.month == 13){
		date.year++;
		date.month = 1;
	}
}

