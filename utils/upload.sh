avrdude -patmega32u4 -cavr109 -P$1 -b57600 -D -Uflash:w:binary/quadcopter.hex:i
