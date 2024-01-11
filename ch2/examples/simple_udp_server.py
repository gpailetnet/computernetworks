from socket import *

HOST_NAME = ""
PORT = 9000
RECV_BUFFER_SIZE = 2

listening_socket = socket(AF_INET, SOCK_DGRAM)
listening_socket.bind((HOST_NAME, PORT))

while True:
    (received_data, client_address) = listening_socket.recvfrom(RECV_BUFFER_SIZE)
    print(f"Got data {received_data}: type {type(received_data)} from client at: {client_address} with type: {type(client_address)}")
    received_str = received_data.decode()
    new_str = received_str.upper()
    listening_socket.sendto(new_str.encode(), client_address)

