import concurrent.futures
import multiprocessing
from socket import *
from sys import exit
import threading
import time

HOST_NAME = ""
HOST_PORT = 9000
RECV_BUFFER_SIZE = 1024
NUM_EXECUTOR_WORKERS = 5
EXECUTOR_TYPE = "process"

CODE_TO_PHRASE = {
    200: "OK",
    301: "Moved Permanently",
    400: "Bad Request",
    404: "Not Found",
    505: "HTTP Version Not Supported",
}

ERR_FILENAME = "error.html"

def display_runtime_info(greeting):
    print(greeting)
    print(
        "I am being called from: ",
        f"Process: {multiprocessing.current_process().name}",
        f"\nThread: {threading.current_thread().name}",
    )


def get_executor(executor_type):
    if executor_type == "thread":
        return concurrent.futures.ThreadPoolExecutor(NUM_EXECUTOR_WORKERS)
    else:
        return concurrent.futures.ProcessPoolExecutor(
            max_workers=NUM_EXECUTOR_WORKERS,
        )


def get_status_line(protocol, status_code):
    return f"{protocol} {status_code} {CODE_TO_PHRASE[status_code]}"


def get_headers(html_output):
    return f"Content-Type: text/html\r\nContent-Length: \
        {len(html_output)}\r\n\r\n"


def get_request_result(html_filename):
    try:
        status_code = 200
        with open(html_filename) as f:
            html_output = ''.join(f.readlines())
    except IOError:
        status_code = 404
        with open(ERR_FILENAME) as f:
            html_output = ''.join(f.readlines())

    return (status_code, html_output)


def html_to_http_response(protocol, html_filename):
    (status_code, html_output) = get_request_result(html_filename)
    status_line = get_status_line(protocol, status_code)
    headers = get_headers(html_output)
    response = status_line + "\r\n" + headers + html_output + "\r\n"

    return response

def handle_connection(connection_socket):
    display_runtime_info("Hello from connection handler!")

    received_bytes = connection_socket.recv(RECV_BUFFER_SIZE)
    received_str = received_bytes.decode()
    print(f"Received message: {received_str}")

    recv_str_list = received_str.split()
    request, location, protocol = recv_str_list[:3]
    filename = location[1:]

    response = html_to_http_response(protocol, filename)

    connection_socket.send(response.encode())
    connection_socket.close()
    time.sleep(5)

display_runtime_info("Hello from the code module!")

if __name__ == '__main__':
    display_runtime_info("Hello from Main Method!")

    server_socket = socket(AF_INET, SOCK_STREAM)
    server_socket.bind((HOST_NAME, HOST_PORT))
    server_socket.listen()

    with get_executor(EXECUTOR_TYPE) as executor:
        while True:
            (connection_socket, client_address) = server_socket.accept()
            executor.submit(handle_connection, connection_socket)
