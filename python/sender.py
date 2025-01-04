import socket
import struct
import json
from multiprocessing import Queue


def sender_process(result_queue: Queue) -> None:
    server_address = '127.0.0.1'
    server_port = 3491
    accumulated_results = []

    while True:
        try:
            result = result_queue.get()

            if result == "STOP":
                print("Sender stopping.")
                break

            accumulated_results.append(result)
        except Exception as e:
            print(f"Error while processing result queue: {e}")
            break

    if not accumulated_results:
        print("No results to send. Sending an empty message...")
        accumulated_results = []

    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
            print(f"Connecting to server {server_address}:{server_port}...")
            client_socket.connect((server_address, server_port))
            print("Connected to server.")

            message = json.dumps(accumulated_results)
            message_size = len(message)

            client_socket.sendall(struct.pack('!I', message_size))

            client_socket.sendall(message.encode())

    except ConnectionRefusedError:
        print("Failed to connect to Getter actor. Make sure the server is running.")
    except Exception as e:
        print(f"An error occurred while sending results: {e}")
