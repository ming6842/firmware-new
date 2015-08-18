#!/usr/bin/python

from __future__ import print_function
import serial
import string
import io
import time
import sys

if __name__ == '__main__':


	port = "/dev/ttyUSB0"
	baudrate = "57600"
	second = 0.0001


	if (len(sys.argv)  < 3 ):
		print("Usage: \n./serial_test.py /dev/ttyUSB0 57600")
		exit()
	elif (len(sys.argv) >= 3):
		port = sys.argv[1]
		baudrate = sys.argv[2]

	print( "open {0}, buadrate {1}, delay in {2} seconds".format(port,baudrate,second))

	ser = serial.Serial(port, baudrate)
	count = 0
	while True:

		ser.write(chr(count) )
		print(count)
		#print (b, end = "")
		if count >=255:
			count = 0
		else:
			count = count + 1

		time.sleep(0.001)







