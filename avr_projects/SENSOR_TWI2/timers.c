#include <timers.h>

/* One second interval interrupt: Assumes SLOW clock_init */
ISR (TIMER2_OVF_vect)
{
	/* Start complete conversion process including GPS */
	if (gpsState == ENABLED)
		gpsConvertState = CAPTURE_GGA;

	/* Start conversion process, just analog sensors */
	else if (gpsState == DISABLED) {
		gpsConvertState = CAPTURE_ADC;
		TCNT2 = 178; // This is for a 10ms update: see below.
	}
}

/* Set one second interval for external RTC crystal */
void interval_clock_init( char clockSpeed)
{

	if (clockSpeed == SLOW) {

		/* Enable asynchonous mode for timer2 */
		set_bit(ASSR, AS2);

		/* Enable 128 prescale */
		set_bit(TCCR2B, CS20);
		set_bit(TCCR2B, CS22);	
	}
	/* This is not supported at the moment. */
	else if (clockSpeed == FAST) {

		/* Disable asynchonous mode for timer2 */
		clear_bit(ASSR, AS2);

		/* Enable 1024 prescale */
		set_bit(TCCR2B, CS20);
		set_bit(TCCR2B, CS21);
		set_bit(TCCR2B, CS22);	
	}

    /* Enable timer2 interrupt on overflow */
    set_bit(TIMSK2, TOIE2);

    /* Clear timer2 overflow flag */
    set_bit(TIFR2, TOV2);
}

