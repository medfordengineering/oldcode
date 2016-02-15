#include <mp3mod.h>

char mp3mod_file_name[13];

void mp3mod_play_all(void)
{
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((MP3_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MP3_PLAYALL;
	TWCR = TWIMASTERSTART;
}

void mp3mod_stop(void)
{
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((MP3_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MP3_STOP;
	TWCR = TWIMASTERSTART;
}

void mp3mod_pause(void)
{
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((MP3_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MP3_PAUSE;
	TWCR = TWIMASTERSTART;
}

void mp3mod_volume(unsigned char volume)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MP3_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MP3_VOLUME;
	twiData[2] = volume;
	TWCR = TWIMASTERSTART;
}

void mp3mod_command(unsigned char command)
{
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MP3_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MP3_COMMAND;
	twiData[2] = command;
	TWCR = TWIMASTERSTART;
}

void mp3mod_play_file(char *filename)
{
	int i;
	while (twiBusy());
	twiMsgSize = 15;
	twiData[0] = ((MP3_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MP3_PLAYFILE;
	for (i = 0; i < 13; i++)
		twiData[i + 2] = filename[i];
	TWCR = TWIMASTERSTART;
}

void mp3mod_play_repeat(char *filename)
{
	int i;
	while (twiBusy());
	twiMsgSize = 15;
	twiData[0] = ((MP3_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MP3_REPEAT;
	for (i = 0; i < 13; i++)
		twiData[i + 2] = filename[i];
	TWCR = TWIMASTERSTART;
}

void mp3mod_read_file_name(int filenum, char *title)
{
	int i;
	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MP3_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MP3_FILENAME;
	twiData[2] = filenum;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 15;
	twiData[0] = ((MP3_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	for (i = 0; i < 13; i++)
		title[i] = twiData[i + 3];
}

unsigned char mp3mod_read_files(void)
{
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((MP3_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MP3_FILES;
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	twiMsgSize = 2;
	twiData[0] = ((MP3_MODULE << 1) | (TWI_READ));
	TWCR = TWIMASTERSTART;

	while (twiBusy());
	return twiData[1];

}

void mp3mod_change_directory(char *filename)
{
	int i;
	while (twiBusy());
	twiMsgSize = 15;
	twiData[0] = ((MP3_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MP3_CHANGE_DIRECTORY;
	for (i = 0; i < 13; i++)
		twiData[i + 2] = filename[i];
	TWCR = TWIMASTERSTART;
}

void mp3mod_up_directory(void)
{
	while (twiBusy());
	twiMsgSize = 1;
	twiData[0] = ((MP3_MODULE << 1) | (TWI_WRITE));
	twiData[1] = CMD_MP3_UP_DIRECTORY;
	TWCR = TWIMASTERSTART;
}
