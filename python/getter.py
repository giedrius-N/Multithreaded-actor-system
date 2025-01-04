import socket
import struct

def main():
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

            print(f"Received from server: {message.decode()}")

            client_socket.sendall(b"ACK")
            print("Sent ACK to server.")

    except ConnectionRefusedError:
        print("Connection failed. Make sure the server is running.")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()
