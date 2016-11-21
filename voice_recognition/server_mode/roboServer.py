#! /usr/bin/python
# -*- coding: utf-8 -*-

# raspirobot server program

import asyncore, socket, time
import raspirobotboard as rp


HOST = '192.168.0.91'   # host IP address
PORT = 9876             # Port No.
BufSize = 1024
NoOfClients = 1

target = rp.RaspiRobot()
lastCommand = "S"

def move_robot(sent):
    move = sent.split()
    flag = False
    keys = ["F","B","L","R","M","S"]
    for c in keys:
       if (c in move):
          flag = True
          break
    #
    if (flag == False):
       return     # do nothing
    #
    if (c == "M"):
       pass
    else:
       lastCommand=c
    #
    actual_move(lastCommand)

def actual_move(c):
    if ("F" == c): 
       target.forward(1.0)
    elif ("B" == c): 
       target.reverse(1.0)
    elif ("L" == c): 
       target.left(0.2)
    elif ("R" == c): 
       target.right(0.2)
    else:
       target.stop()

def send_all(message):
   for x in Clients.values():
        x.send(message)

class EchoHandler(asyncore.dispatcher_with_send):
    def handle_read(self):
        data = self.recv(BufSize)
        if data:
            print data
            if "Q" in data:
                target.stop()
            # if data=="balse":
                # self.send('BYE NOW *** SERVER HALTS')
                # send_all('BYE NOW *** SERVER HALTS')       #
                # time.sleep(1)
                exit(0)
            else:
                # send_all(data)                             #
                # self.send(data)
                move_robot(data)
 
class EchoServer(asyncore.dispatcher):
    def __init__(self, host, port):
        asyncore.dispatcher.__init__(self)
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        self.set_reuse_addr()
        self.bind((HOST,PORT))
        self.listen(NoOfClients)
    
    def handle_accept(self):
        pair = self.accept()
        if pair is None:
            sock, addr = pair				#
            Clients.pop(sock)				#
            # pass
        else:
            sock, addr = pair
            print('Incoming connection from %s' % repr(addr))
            handler = EchoHandler(sock)
            Clients[sock]=handler			#

server = EchoServer(HOST,PORT)
asyncore.loop()

