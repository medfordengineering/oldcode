#####################################
# Machine Science Makefile template # 
#####################################
##Fuse Settings for bootloader
#w efuse 0 0xf8
#w hfuse 0 0xdf
#w lfuse 0 0xe2
#d efuse to view settings

##Fuse Settings for 20mHz and BOD_1.8V
# efuse 0xF8 Standard Bootloader config
# hfuse 0xDE With BOD_1.8V
# lfuse 0xCE 8+ mHz Clock
 
## Name of project
PROJECT = darla_twi

## Define freguency of oscillator
F_CPU = 20000000L

## Define MCU
MCU = atmega168

## Define programmer MCU
PROGRAMMER_MCU = m168

## Define programmer ID
PROGRAMMER_ID = dragon_isp
#PROGRAMMER_ID = stk500v2

## Define programmer port
PROGRAMMER_PORT = usb
#PROGRAMMER_PORT = /dev/ttyS1

## Define programmer bitclock
#PROGRAMMER_BC = 250
PROGRAMMER_BC = 8

## List of executables
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRSIZE = avr-size
AVRDUDE = avrdude

## Compile for MCU
COMMON = -mmcu=$(MCU)

## Define debugging format
DEBUG = dwarf-2

## Define optimization level 
OPTIMIZE = 0

## Define warning level
WARNING = -Wall -Wstrict-prototypes -Wno-char-subscripts 

## Define fields
FIELDS = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums

## Define command line options
COMMANDLINE = FOSC=$(F_CPU)

## Set all compile options
CFLAGS = $(COMMON)
CFLAGS += -O$(OPTIMIZE)
CFLAGS += -g$(DEBUG)
CFLAGS += $(WARNING)
CFLAGS += $(FIELDS)
CFLAGS += -D$(COMMANDLINE)

## Define Print libaries
# a) Not defined
 PRINTF_LIB = 

# b) Minimalistic printf version
# PRINTF_LIB = -Wl,-u,vfprintf -lprintf_min
#
# c) Floating point printf version (requires MATH_LIB = -lm below)
#PRINTF_LIB = -Wl,-u,vfprintf -lprintf_flt

## Additional Math lib
#MATH_LIB = -lm
MATH_LIB = 

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += $(PRINTF_LIB) $(MATH_LIB)

## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings

## Include Directories
INCLUDES = -I../avr_inc -I../avr_src
#INCLUDES = -I../avr_inc 

## Library Directories
LIBDIRS = -L../avr_lib

## Libraries
LIBS =	
#LIBS =	-l8usart -l8delay -l8adc -l8lcd

## Objects that must be built in order to link
OBJECTS = $(PROJECT).o

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(PROJECT).elf $(PROJECT).hex size

## Compile
$(PROJECT).o: $(PROJECT).c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link (LDFLADS, specifcally -lm MUST go last!)
$(PROJECT).elf: $(OBJECTS)
	 $(CC) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(PROJECT).elf $(LDFLAGS)

%.hex: $(PROJECT).elf
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(PROJECT).elf
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(PROJECT).elf
	avr-objdump -h -S $< > $@

size: ${PROJECT}.elf
	@echo
	@$(AVRSIZE) -Bdt $(PROJECT).elf

writeflash:
	$(AVRDUDE) -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -B $(PROGRAMMER_BC) -P $(PROGRAMMER_PORT) -e  -U flash:w:$(PROJECT).hex 

writefirm:
	$(AVRDUDE) -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -B $(PROGRAMMER_BC) -u -P $(PROGRAMMER_PORT) -e \
	-U flash:w:$(PROJECT).hex \
	-U efuse:w:0x0:m \
	-U hfuse:w:0xdd:m \
	-U lfuse:w:0xef:m

runterminal:
	$(AVRDUDE) -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -B $(PROGRAMMER_BC) -P $(PROGRAMMER_PORT) -t

setfuse:
	$(AVRDUDE) -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -B $(PROGRAMMER_BC) -u -P $(PROGRAMMER_PORT) -t

fuseinit:
	$(AVRDUDE) -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -B $(PROGRAMMER_BC) -u -P $(PROGRAMMER_PORT) \
	-U efuse:w:0x0:m \
	-U hfuse:w:0xdf:m \
	-U lfuse:w:0xe2:m

debug:
	$(AVRDUDE) -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -B $(PROGRAMMER_BC) -u -P $(PROGRAMMER_PORT) \
	-U efuse:w:0x1:m \
	-U hfuse:w:0x9f:m \
	-U lfuse:w:0xe2:m

bugout:
	$(AVRDUDE) -p $(PROGRAMMER_MCU) \
	-c $(PROGRAMMER_ID) -B $(PROGRAMMER_BC) -P $(PROGRAMMER_PORT) -F

## Clean PROJECT
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(PROJECT).elf dep/* $(PROJECT).hex $(PROJECT).eep


