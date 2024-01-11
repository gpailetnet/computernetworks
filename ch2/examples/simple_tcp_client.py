from socket import *

HOST_NAME=""
HOST_PORT = 9000
RECV_BUFFER_SIZE = 1024

while True:
    client_socket = socket(AF_INET, SOCK_STREAM)
    client_socket.connect((HOST_NAME, HOST_PORT))
    request_str = input("Enter something to send!\n")
    client_socket.sendto(request_str.encode(), (HOST_NAME, HOST_PORT))
    (received_data, host_address) = client_socket.recvfrom(RECV_BUFFER_SIZE)
    print(f"Got data {received_data}: type {type(received_data)} from host at: {host_address} with type: {type(host_address)}")
    client_socket.close()
    print("Closed connection!")