# network client logic
import socket, io, struct

def connect(host, port, timeout):
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((host, port))
	s.settimeout(timeout)
	return s

def send(s, buffer) :
	s.send(buffer)

def recv(s) :
	data=s.recv(1024)	
	return data



