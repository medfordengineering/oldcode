/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2007.  All rights reserved
	
 Version Date: Mon Dec 17 17:10:43 EST 2007
-----------------------------------------------------------------
============================================================== */

#include "ump3.h"

int uMP3_count_title(void)
{
	char value = 0;
	int total_titles = 0;
	usart_text("FC L /");
	usart_write(13);

   	while( isdigit( value = usart_read()) == FALSE);
	while(( value = usart_read() ) != '>'){
		if ( value == CARRIAGE_RETURN)
			total_titles++;
	}
	return total_titles;
}
void uMP3_read_title(char * title, char title_counter)
{
	usart_text("FC L /");
	usart_write(13);
	while(title_counter){
		if ( usart_read() == CARRIAGE_RETURN)
			title_counter--;
	}
   	while( isalpha( *title =  usart_read() ) == FALSE);
	while( ( *++title = usart_read() ) != '.');
	*title = '\0';
	while( usart_read() != '>' );
}

void uMP3_play( char * track)
{
	usart_text("PC F /");
	usart_text(track);
	usart_text(".mp3");
	usart_write(13);
}


