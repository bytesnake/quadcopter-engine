#!/bin/sh

cat /dev/ttyACM0 > test.dat &

sleep 5;

kill $!	

./plot
