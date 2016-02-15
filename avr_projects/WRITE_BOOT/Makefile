###############################################################################
# Makefile for the project $(PROJECT)
###############################################################################

##Fuse Settings for bootloader
#w efuse 0 0xf8
#w hfuse 0 0xdf
#w hfuse 0 0xdd = 2.7V brownout
#w lfuse 0 0xe2
#2 lfuse 0 0xde = 8 + Mhz
#d efuse to view settings
 
## General Flags
## THIS FILE IS NOT USED SEE BELOW
PROJECT = AT168diec4
MCU = atmega168

## Programmer
PROGRAMMER_MCU = m168
#PROGRAMMER_MCU = m164p

PROGRAMMER_ID = avrisp2
#PROGRAMMER_ID = dragon_isp
#PROGRAMMER_ID = avrispmkII

PROGRAMMER_PORT = usb

## Define programmer bitclock
PROGRAMMER_BC = 250
#PROGRAMMER_BC = 70
#PROGRAMMER_BC = 20
#PROGRAMMER_BC = 8

## Executables
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRSIZE = avr_size
AVRDUDE = avrdude


## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -g -O0
#CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += 

## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings

## Include Directories
INCLUDES = -I../avr_inc -I../avr_src

## Library Directories
LIBDIRS = -L../avr_lib

## Libraries
LIBS = -lmx 

## Objects that must be built in order to link
OBJECTS = $(PROJECT).o

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
#all: $(PROJECT) $(PROJECT).hex $(PROJECT).eep size
all: $(PROJECT).elf $(PROJECT).hex size

## Compile
$(PROJECT).o: $(PROJECT).c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(PROJECT).elf: $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(PROJECT).elf

%.hex: $(PROJECT).elf
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(PROJECT).elf
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(PROJECT).elf
	avr-objdump -h -S $< > $@

size: ${PROJECT}.elf
	@echo
#	@$(AVRSIZE) -Bd $(PROJECT).elf

writeflash:
	$(AVRDUDE)  -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -B $(PROGRAMMER_BC) -P $(PROGRAMMER_PORT) -e  -U flash:w:$(PROJECT).hex 

writeboot8:
	$(AVRDUDE)  -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -B 8 -P $(PROGRAMMER_PORT) -e -u \
	-U flash:w:XIPmod04-031010.hex \
	-U efuse:w:0x0:m \
	-U hfuse:w:0xde:m \
	-U lfuse:w:0xc2:m \
	-U lock:w:0x0f:m

lilly:
	$(AVRDUDE)  -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -B 8 -P $(PROGRAMMER_PORT) -e -u \
	-U flash:w:XIPmod04-031010.hex \
	-U efuse:w:0x0:m \
	-U hfuse:w:0xdd:m \
	-U lfuse:w:0xe2:m \
	-U lock:w:0x0f:m


writeboot20:
	$(AVRDUDE)  -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -B 20  -P $(PROGRAMMER_PORT) -e -u \
	-U flash:w:XIPmod20-031010.hex \
	-U efuse:w:0x0:m \
	-U hfuse:w:0xdd:m \
	-U lfuse:w:0xef:m \
	-U lock:w:0x0f:m

writebootpin:
	$(AVRDUDE)  -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -B 100  -P $(PROGRAMMER_PORT) -e -U flash:w:pinpoint0.hex 

writebootDiec:
	$(AVRDUDE)  -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -B $(PROGRAMMER_BC)  -P $(PROGRAMMER_PORT) -e -u \
	-U flash:w:ATmegaBOOT_168_diecimila.hex \
	-U efuse:w:0x0:m \
	-U hfuse:w:0xdd:m \
	-U lfuse:w:0xef:m

runterminal:
	$(AVRDUDE) -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -P $(PROGRAMMER_PORT) -t

setfuse:
	$(AVRDUDE) -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -u -P $(PROGRAMMER_PORT) -t

fuseinit:
	$(AVRDUDE) -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -u -P $(PROGRAMMER_PORT) \
	-U efuse:w:0x0:m \
	-U hfuse:w:0xdf:m \
	-U lfuse:w:0xe2:m

readflash:
	$(AVRDUDE) -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -B 50 -P $(PROGRAMMER_PORT) \
	-U flash:r:dump.hex:i

verifyflash8:
	$(AVRDUDE) -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -B 20 -P $(PROGRAMMER_PORT) \
	-U flash:v:XIPmod04-031010.hex 

verifyflash20:
	$(AVRDUDE) -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -B 20 -P $(PROGRAMMER_PORT) \
	-U flash:v:XIPmod20-031010.hex 

## Clean PROJECT
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(PROJECT).elf dep/* $(PROJECT).hex $(PROJECT).eep

## Other dependencies
#-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

