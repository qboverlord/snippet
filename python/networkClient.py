import socket

class tcpClient:

    sock = 0
    def __init__(self, sock=None):
        if sock is None:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        else:
            self.sock = sock

    def connect(self, host, port):
        self.sock.connect((host, port))

    def send(self, msg):
        sent = self.sock.sendall(msg)
        if sent == 0:
            raise RuntimeError("send socket connection broken")

    def recv(self, howMany=2048):
        data = self.sock.recv(howMany)
        if data == '':
            print 'recv chunk error'
            raise RuntimeError("recv socket connection broken")
        return data

