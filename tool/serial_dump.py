#!/usr/bin/python

import serial
import string
import io
import time
import sys

if __name__ == '__main__':


	port = "/dev/ttyUSB0"
	baudrate = "57600"
	second = 0.1


	if (len(sys.argv) < 3):
		print("Usage: serial_dump.py /dev/ttyUSB0 57600")
		exit()
	elif (len(sys.argv) == 3):
		port = sys.argv[1]
		baudrate = sys.argv[2]
	elif (len(sys.argv) == 4):
		port = sys.argv[1]
		baudrate = sys.argv[2]
		second = float(sys.argv[3])
	

	print( "open {0}, buadrate {1}, delay in {2} seconds".format(port,baudrate,second))

	ser = serial.Serial(port, baudrate);
	
	with open("gps.log","rb") as f:
		string = f.read()

		for byte in string: 

			ser.write(byte)
			print_byte = ":".join("{:02x}".format(ord(c)) for c in byte)

			print ("{0} ".format(print_byte))

			time.sleep(second)

        ser.close()






