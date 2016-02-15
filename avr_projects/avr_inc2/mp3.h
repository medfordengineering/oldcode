/* -------------------------------------------------------------
 Machine Science API
 (c) Copyright Machine Science, 2006.  All rights reserved

 Version Date:   Thu Jun 26 11:51:34 EDT 2008
 
-----------------------------------------------------------------
============================================================== */
#ifndef _MP3_H_
#define _MP3_H_

#include <usart.h>
#include <delay.h>

#define  	MP3_UP_DIRECTORY		0x02
#define 	MP3_CHANGE_DIRECTORY	0x02
#define 	MP3_PLAY_FILE			0x1D

#define  	MP3_SHORT_MODE         	0x10
#define  	MP3_STOP            	0x20
#define  	MP3_PLAY_ALL       	 	0x21
#define 	MP3_PLAY_ALLREPEAT		0x8A
#define  	MP3_PLAY_REPEAT     	0x89
#define  	MP3_PLAY_RANDOM     	0x8F
#define  	MP3_SKIP_FORWARD    	0x25
#define  	MP3_SKIP_BACKWARD   	0x26
#define  	MP3_SKIP_DIRECTORY  	0x8E
#define  	MP3_PAUSE           	0x8B
#define  	MP3_FORWARD_FIVE    	0x8C
#define  	MP3_BACK_FIVE       	0x8D
#define  	MP3_DIRECTORY       	0x01

#define 	TMB_MAKE_DIRECTORY		0x06
#define 	TMB_OPEN_FILE			0x09
#define 	TMB_CLOSE_FILE			0x0A

extern void mp3_command(unsigned char );
extern void mp3_file(unsigned char , char * );
extern void mp3_file_name(char * ,  char );
extern char mp3_total_files( void );
extern void write_data(char);
extern void mp3_volume( char );
extern void mp3_init(void);
	
#endif
