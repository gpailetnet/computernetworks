from socket import *
import time

"""
Packet Format:
seq time
"""

HOST_NAME = ""
HOST_PORT = 9092
RECV_BUF_SIZE = 1024

server_socket = socket(AF_INET, SOCK_DGRAM)
server_socket.bind((HOST_NAME, HOST_PORT))
server_socket.settimeout(5)

last_seq = -1

while True:
    try:
        (data, client_address) = server_socket.recvfrom(RECV_BUF_SIZE)
        cur_time = time.time()
        [seq_str, time_str] = data.decode().split()
        seq, client_time = int(seq_str), float(time_str)
        time_diff = cur_time - client_time
        print(f"Received packet: {seq}; time difference: {time_diff * 1000:0.3f} ms")
        if seq > last_seq + 1:
            print(
                f"Lost packets: "
                f"{[lost_seq for lost_seq in range(last_seq + 1, seq)]}"
            )
        last_seq = seq
    except TimeoutError:
        print("the client is dead!!!!")
        break

server_socket.close()
