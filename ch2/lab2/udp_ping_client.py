import random
from socket import *
import time

HOST_NAME = ""
HOST_PORT = 9000
RECV_BUF_SIZE = 1024

MESSAGE_SIZE = 64
NUM_MESSAGES = 20

client_socket = socket(AF_INET, SOCK_DGRAM)
client_socket.settimeout(1)

rtts_ms = []

def std_dev(entries):
    avg = sum(entries) / len(entries)
    return (sum(map(lambda x: (x - avg) ** 2, entries))) ** 0.5

for seq in range(NUM_MESSAGES):
    message = random.randbytes(MESSAGE_SIZE)
    start = time.time()
    client_socket.sendto(message, (HOST_NAME, HOST_PORT))
    try:
        (received_message, host_address) = client_socket \
            .recvfrom(RECV_BUF_SIZE)
        finish = time.time()
        rtt = (finish - start) * 1000
        output = f"{len(received_message)} " \
            f"bytes from {host_address[0]}: " \
            f"icmp_seq={seq} " \
            f"time={rtt:.3f} ms"
        print(output)
        rtts_ms.append(rtt)
    except TimeoutError as e:
        print(f"Request timed out")
print('--- ping statistics ---')
print(
    f"{NUM_MESSAGES} transmitted, "
    f"{len(rtts_ms)} received, "
    f"{((1 - len(rtts_ms) / NUM_MESSAGES) * 100):.1f}% packet loss"
)
print(
    f"round-trip min/avg/max/stddev = "
    f"{min(rtts_ms):.3f}/{(sum(rtts_ms)/len(rtts_ms)):.3f}/"
    f"{max(rtts_ms):.3f}/{std_dev(rtts_ms):.3f} ms"
)

