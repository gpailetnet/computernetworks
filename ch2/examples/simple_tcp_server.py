from socket import *

HOST_NAME = ""
PORT = 9000
RECV_BUFFER_SIZE = 1024

server_socket = socket(AF_INET, SOCK_STREAM)
server_socket.bind((HOST_NAME, PORT))
server_socket.listen()

while True:
    (connection_socket, client_address) = server_socket.accept()
    received_data = connection_socket.recv(RECV_BUFFER_SIZE)
    # (received_data, client_address) = listening_socket.recvfrom(RECV_BUFFER_SIZE)
    print(f"Got data {received_data}: type {type(received_data)} from client at: {client_address} with type: {type(client_address)}")
    received_str = received_data.decode()
    new_str = received_str.upper()
    connection_socket.send(new_str.encode())
    connection_socket.close()
    print("Closed connection!")

