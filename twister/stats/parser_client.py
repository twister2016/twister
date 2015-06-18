import socket
import sys

port = 5555 #same as defined by C constructor:
ip='127.0.0.1'
s = socket.socket()
s.connect((ip,port))
s.send("my request\r")
print s.recv(256)
s.close()
