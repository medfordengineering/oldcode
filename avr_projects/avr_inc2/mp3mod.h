/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue Mar 30 14:02:15 EDT 2010
-----------------------------------------------------------------
============================================================== */

#ifndef _MP3MOD_H_
#define _MP3MOD_H_

#ifndef _SLAVE
	#include <twi_master.h>
#endif

#include <delay.h>

#define CMD_MP3_PLAYALL				0x00
#define CMD_MP3_PLAYFILE			0x01
#define CMD_MP3_FILENAME			0x02	
#define CMD_MP3_STOP				0x03
#define CMD_MP3_PAUSE				0x04
#define CMD_MP3_VOLUME				0x05
#define CMD_MP3_DIRECTORY			0x06
#define CMD_MP3_FILES				0x07
#define CMD_MP3_CHANGE_DIRECTORY 	0x08
#define CMD_MP3_UP_DIRECTORY 		0x09
#define CMD_MP3_REPEAT				0x0A
#define CMD_MP3_COMMAND				0x10

extern void mp3mod_play_all(void);
extern void mp3mod_play_repeat(char *);
extern void mp3mod_command(unsigned char);
extern void mp3mod_stop(void);
extern void mp3mod_pause(void);
extern void mp3mod_play_file(char *);
extern void mp3mod_read_file_name(int, char *);
extern unsigned char mp3mod_read_files(void);
extern void mp3mod_change_directory(char *);
extern void mp3mod_up_directory(void);
extern void mp3mod_volume(unsigned char );

#endif
