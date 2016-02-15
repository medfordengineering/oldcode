#include <calibrate.h>

// Target count = T2 Compare Value * FOSC / T2 Clock
unsigned int osccal_calibrate( uint16_t lthresh, uint16_t hthresh) 
{
	unsigned char calibrate = FALSE;
	unsigned int temp = 0;
	
	// THIS DOES NOT WORK
	// Prescale system clock by 2
	//CLKPR = (1<<CLKPCE);
	//CLKPR = 0x01;
	
	TIMSK2 = 0; 			//disable OCIE2A and TOIE2, see page 140 of the ATMega169 docs

	ASSR = (1<<AS2);		//select asynchronous operation of timer2 (32.768kHz)
	
	OCR2A = 100;			// set timer2 compare value 

	TIMSK0 = 0; 			// delete any interrupt sources
		
	TCCR1B = (1 << CS10); 	// start timer1 with no prescaling
	TCCR2B = (1 << CS20); 	// start timer2 with no prescaling

	while((ASSR & 0x01) | (ASSR & 0x04));   	//wait for TCN2UB and TCR2BUB to be cleared

	_delay_loop_2(1000);	// wait for external crystal to stabilise
	
	while(!calibrate) {
		cli(); // disable global interrupt
		
		TIFR1 = 0xFF;   // delete TIFR1 flags
		TIFR2 = 0xFF;   // delete TIFR2 flags
		
		TCNT1 = 0;
		TCNT2 = 0;  	// clear timer2 counter
   			
		while ( !(TIFR2 & (1<<OCF2A)) );   // wait for timer2 compareflag

		TCCR1B = 0; // stop timer1

		sei(); // enable global interrupt
		
		if ( (TIFR1 & (1<<TOV1)) ) {
			temp = 0xFFFF;  	// if timer1 overflows, set the temp to 0xFFFF
		} 
		else {   // read out the timer1 counter value
			temp = TCNT1;
		}
		if (temp > hthresh) {
			OSCCAL--;   // the internRC oscillator runs to fast, decrease the OSCCAL
		}
		else if (temp < lthresh) {
			OSCCAL++;   // the internRC oscillator runs to slow, increase the OSCCAL
		} 
		else {
			calibrate = TRUE;   // the interRC is correct
		}

		if( !calibrate ) {
			TCCR1B = (1<<CS10); // start timer1
		} 
		else {
			TCCR1B = (0 << CS10);
			TCCR2B = (0 << CS20);
		}
	}
	
	// Reset system clock prescale to 0 
	//CLKPR = (1<<CLKPCE);
	//CLKPR = 0x00;
	
	// Set FOSC based on calibration, not implemented
	// Should return a percentage of accuracy
	//FOSC = (t1Count << 12) / 25;
	return temp;
}
