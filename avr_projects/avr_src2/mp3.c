/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:    Thu Jun 26 11:50:02 EDT 2008
-----------------------------------------------------------------
============================================================== */

#include "mp3.h"

void mp3_file_name(char * title, char title_counter)
{
	title_counter += 1;

    mp3_command(MP3_DIRECTORY);
	while (usart_read() != '.');
	while (usart_read() != '.');
	while (usart_read() != '.');

    while (title_counter) {
        if (usart_read() == CARRIAGE_RETURN)
            title_counter--;
    }
    while ((*title = usart_read()) != CARRIAGE_RETURN) 
		title++;
    *title = '\0';
}
/*
void mp3_file_name(char * title, char * type, char title_counter)
{
	*type = 0;
    mp3_command(MP3_DIRECTORY);
	while( usart_read() != '.');
    while(title_counter){
        if ( usart_read() == CARRIAGE_RETURN)
            title_counter--;
    }
    while( ( *title = usart_read() ) != CARRIAGE_RETURN){
		if (*title == ' ') *type += 2;
		if (*title == '.') *type += 1;
		if (*title == '>') *type += 5;
		title++;
	}	
    *title = '\0';
}
*/
char mp3_total_files( void )
{
	char value;
	char tracks_counter = 0;
	mp3_command(MP3_DIRECTORY);
	while( usart_read() != '.');
	while( usart_read() != '.');
	while( usart_read() != '.');
	while( (value = usart_read()) != '>'){
		if( value == CARRIAGE_RETURN ) tracks_counter++;
	}
    return --tracks_counter;
}

void mp3_file(unsigned char command, char * fileName)
{
	usart_write(command);
	usart_write(0x20);
	usart_text(fileName);
	usart_write(CARRIAGE_RETURN);
}

void mp3_command(unsigned char command)
{
    usart_write(command);
	if(command == MP3_UP_DIRECTORY){
		usart_write(0x20);
		usart_write(0x2E);
		usart_write(0x2E);
	}
    usart_write(CARRIAGE_RETURN);
}

void mp3_volume( char volume )
{
//	volume = 0xFF - volume;
	volume = (100 - volume) * 1.5;
	usart_write(0x88);
	usart_write(0x20);
	usart_write( volume );
	usart_write(CARRIAGE_RETURN);
}

void write_data(char nbytes)
{
	usart_write(0x08);
	usart_write(0x20);
	usart_write(0x00);
	usart_write(0x00);
	usart_write(0x00);
	usart_write(nbytes);
	usart_write(CARRIAGE_RETURN);
}

void mp3_init(void)
{
	usart_init();
	delay_ms(2000);
	mp3_command(MP3_SHORT_MODE);
}
