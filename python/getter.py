import socket

def main():
    server_address = '127.0.0.1'
    server_port = 3490

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        print(f"Connecting to server {server_address}:{server_port}...")
        client_socket.connect((server_address, server_port))

        greeting = client_socket.recv(1024).decode()
        print(f"Received from server: {greeting}")

        ack_message = "ACK"
        client_socket.sendall(ack_message.encode())
        print("Sent ACK to server.")

if __name__ == "__main__":
    main()
