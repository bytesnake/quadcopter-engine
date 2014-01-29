CC=avr-gcc
OBJCPY=avr-objcopy

DEV=/dev/ttyACM0

TYPE=atmega32u4
CPU_SPEED=16000000UL
BAUD=9600

SOURCES=$(shell find source -type d -name ioplatforms -prune -o -name "*.c" -print | xargs)
OUTPUT=binary/quadcopter

default:
	$(CC) -Os -Wl,-u,vfprintf -lprintf_flt -lm -mmcu=$(TYPE) -DUSB_VID=0x2341 -DUSB_PID=0x8041 -DF_CPU=$(CPU_SPEED) -DBAUD=$(BAUD) $(SOURCES) -o $(OUTPUT)
	$(OBJCPY) -O ihex -R .eeprom $(OUTPUT) $(OUTPUT).hex
upload:
	$(shell python2 utils/upload.py $(DEV))
monitor:
	$(shell screen $(DEV) $(BAUD))
