"""
Process that receives data from the server and puts it in the work queue.
"""

import socket
import struct
import json
from multiprocessing import Queue

def getter_process(work_queue: Queue) -> None:
    server_address = '127.0.0.1'
    server_port = 3490

    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
            print(f"Connecting to server {server_address}:{server_port}...")
            client_socket.connect((server_address, server_port))
            print("Connected to server.")

            size_data = client_socket.recv(4)
            if not size_data:
                print("Failed to receive message size.")
                return

            message_size = struct.unpack('!I', size_data)[0]

            message = b""
            while len(message) < message_size:
                chunk = client_socket.recv(message_size - len(message))
                if not chunk:
                    break
                message += chunk

            decoded_message = message.decode()

            ack_message = b"ACK"
            size_prefix = struct.pack('!I', len(ack_message))
            client_socket.sendall(size_prefix + ack_message)
            print("Sent ACK to server.")

            try:
                items = json.loads(decoded_message)
                for item in items:
                    work_queue.put(item)
            except json.JSONDecodeError as e:
                print(f"Failed to parse JSON: {e}")

    except ConnectionRefusedError:
        print("Connection failed. Make sure the server is running.")
    except Exception as e:
        print(f"An error occurred: {e}")
