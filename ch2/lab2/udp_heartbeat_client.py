import random
from socket import *
import time

HOST_NAME = ""
HOST_PORT = 9092
NUM_MESSAGES = 10
LOSS_RATE = 0.3


client_socket = socket(AF_INET, SOCK_DGRAM)

for seq in range(NUM_MESSAGES):
    if random.random() > LOSS_RATE:
        cur_time = time.time()
        data_str = f"{seq} {cur_time}"
        data = data_str.encode()
        client_socket.sendto(data, (HOST_NAME, HOST_PORT))
    time.sleep(1)