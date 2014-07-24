import serial
import string
import io
import time

if __name__ == '__main__':

	ser = serial.Serial('/dev/ttyUSB0', 57600, timeout=1);
	
	with open("gps.log","rb") as f:
		string = f.read()

		for byte in string: 

			ser.write(byte)
			print_byte = ":".join("{:02x}".format(ord(c)) for c in byte)
			print('{0}'.format(print_byte) )
			time.sleep(0.01)

        ser.close()






