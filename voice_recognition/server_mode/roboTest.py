#! /usr/bin/python
# server3.py

import asyncore, socket, time

HOST = '192.168.0.91'
PORT = 9876
BufSize = 1024
NoOfClients = 10

class EchoHandler(asyncore.dispatcher_with_send):
    def handle_read(self):
        data = self.recv(BufSize)
        if data:
                self.send(data)
                print data
                if "Q" in data:
                   exit(0)

class EchoServer(asyncore.dispatcher):
    def __init__(self, host, port):
        asyncore.dispatcher.__init__(self)
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        self.set_reuse_addr()
        self.bind((HOST,PORT))
        self.listen(NoOfClients)
    #
    def handle_accept(self):
        pair = self.accept()
        if pair is None:
            pass
        else:
            sock, addr = pair
            print('Incoming connection from %s' % repr(addr))
            handler = EchoHandler(sock)

server = EchoServer(HOST,PORT)
asyncore.loop()
