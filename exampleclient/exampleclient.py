#!/usr/bin/env python3

import socket

TCP_IP = '192.168.0.101'
TCP_PORT = 8266

def send(cmd):
	s.send(bytearray(cmd, 'ascii'))
	print("Sent.")
	data = s.recv(1024)
	print("Received:");
	print("\tHex:" + ''.join('{:02x}'.format(x) for x in data));
	try:
		print("\tASCII: " + data.decode("ascii"))
	except:
		print("\tASCII: No ascii decoding.")

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
	s.connect((TCP_IP, TCP_PORT))
	send("Message from Python.")
