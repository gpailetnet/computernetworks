from socket import *
import sys

METHOD = "GET"
HTTP_VERSION = "HTTP/1.1"
RECV_BUFFER_SIZE = 1024

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Must  supply 3 arguments!")
    else:
        server_host = sys.argv[1]
        server_port = int(sys.argv[2])
        filename = sys.argv[3]

        client_socket = socket(AF_INET, SOCK_STREAM)
        client_socket.connect((server_host, server_port))

        request_line = f"{METHOD} {filename} {HTTP_VERSION}\r\n"
        headers = f"Accept-language: fr\r\nHost: {server_host}\r\n"
        termination = "\r\n"

        http_request = f"{request_line}{headers}{termination}"

        client_socket.send(http_request.encode())
        http_response = client_socket.recv(
            RECV_BUFFER_SIZE
        ).decode()

        print(f"Got response:\n{http_response}")
