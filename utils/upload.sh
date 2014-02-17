/usr/share/arduino/hardware/tools/avrdude -q -V -D -p atmega32u4 -C /usr/share/arduino/hardware/tools/avrdude.conf -c avr109 -b 57600 -P$1 -Uflash:w:binary/quadcopter.hex:i
