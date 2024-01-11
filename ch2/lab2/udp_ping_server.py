import random
from socket import *

# Ping back 70% of messages

HOST_NAME = ""
HOST_PORT = 9000
RECV_BUF_SIZE = 1024

server_socket = socket(AF_INET, SOCK_DGRAM)
server_socket.bind((HOST_NAME, HOST_PORT))

while True:
    (message, client_address) = server_socket \
        .recvfrom(RECV_BUF_SIZE)
    if random.random() > 0.3:
        server_socket.sendto(message, client_address)
