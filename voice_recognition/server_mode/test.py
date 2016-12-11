#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket, re
import serial

def show(line):
   for x in line:
      print x," ",
   print


def catchSent():
  flag=False
  answer=[]
  while True:
    recv_data = clientsock.recv(512)
    if re.search('<RECOGOUT>',recv_data):
       flag=True
    elif (flag==True):
       tmp = re.findall('WORD=".+"',recv_data)
       for one in tmp:
          answer.append(one.split('"')[1])
       if re.search('/RECOGOUT>',recv_data):
          return(answer)


host = 'localhost'
port = 10500

clientsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientsock.connect((host, port))
# com = serial.Serial('/dev/ttyAMA0',57600,timeout=10)
while True:
     sent = catchSent()
     show(sent)
    # show(sent[1:-1])
     # com.write(recv_data)
