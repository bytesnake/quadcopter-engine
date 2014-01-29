#!/usr/bin/env python

import serial, sys, time

from subprocess import call

serialPort = sys.argv[1]
#print(serialPort)

call(["stty", "-F", serialPort, "hupcl"]);

time.sleep(1)

conn = serial.Serial(
    port=serialPort,
    baudrate=1200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)
conn.isOpen()
conn.close() # always close port

time.sleep(2)

call(["bash", "./utils/upload.sh", serialPort]);
