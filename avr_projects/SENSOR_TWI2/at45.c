#include <at45.h>

void at45_busy(void);

void at45_busy(void)
{
    while (!(at45_status() & STATUS_READY_BIT));
}

unsigned char at45_status(void)
{
	unsigned char data;

	spi_start_command();

	spi_transfer(AT45_STATUS);
	data = spi_transfer(0x00);

	spi_end_command();

	return data;
}

status at45_buffer_write(unsigned int byte_address, unsigned char *data, unsigned char size)
{
	unsigned int i = 0;
	
	spi_start_command();

	spi_transfer(AT45_WRITE_TO_BUFFER_1);

	spi_transfer(0x00);

	spi_transfer((unsigned char) (byte_address >> 8));
	spi_transfer((unsigned char) (byte_address));

	while (i < size)
		spi_transfer(data[i++]);

	spi_end_command();

	return OK;
}

status at45_buffer_read(unsigned int byte_address, unsigned char *data, unsigned char size)
{
	  unsigned int i = 0;

	  spi_start_command();
	  spi_transfer(AT45_READ_FROM_BUFFER_1);
	  spi_transfer(0x00);
	  spi_transfer((unsigned char) (byte_address >> 8));
	  spi_transfer((unsigned char) (byte_address));
	  spi_transfer(0x00);
	  while (i < size)
		  data[i++] = spi_transfer(0x00);

	  spi_end_command();
	  return OK;
}

status at45_page_write(unsigned int pageAddress)
{
	spi_start_command();

	spi_transfer(AT45_WRITE_FROM_BUFFER_1);
	spi_transfer ((unsigned char) (pageAddress >> 7));
	spi_transfer ((unsigned char) (pageAddress << 1));
	spi_transfer ((unsigned char) (0x00));

	spi_end_command();

	at45_busy();

	return OK;
}

status at45_page_read(	unsigned int pageAddress, 
						unsigned int byteAddress, 
						volatile unsigned char *data, 
						unsigned int size)
{
	unsigned int i = 0;

	spi_start_command();

	spi_transfer(AT45_READ_FROM_PAGE);
	spi_transfer ((unsigned char) (pageAddress >> 7));
	spi_transfer ((unsigned char) (pageAddress << 1) + (byteAddress >> 8));
	spi_transfer((unsigned char)(byteAddress));
	spi_transfer(0x00);
	spi_transfer(0x00);
	spi_transfer(0x00);
	spi_transfer(0x00);

	while (i < size)
		data[i++] = spi_transfer(0x00);

	spi_end_command();

	return OK;
}

status at45_chip_erase(void)
{
	int i;

	for (i = 0; i < 256; i++){
		at45_block_erase(i);
	}

	return OK;
}

status at45_block_erase(unsigned int blockAddress)
{
	spi_start_command();

	spi_transfer(AT45_BLOCK_ERASE);
	spi_transfer ((unsigned char) (blockAddress >> 4));
	spi_transfer ((unsigned char) (blockAddress << 4));
	spi_transfer ((unsigned char) (0x00));

	spi_end_command();

	at45_busy();

	return OK;
}

