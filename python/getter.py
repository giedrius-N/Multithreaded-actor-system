import socket

def main():
    server_address = '127.0.0.1' 
    server_port = 3490         

    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
            print(f"Connecting to server {server_address}:{server_port}...")
            
            client_socket.connect((server_address, server_port))
            print("Connected to server.")

            while True:
                try:

                    message = client_socket.recv(1024)
                    if not message:
                        print("Server closed the connection.")
                        break

                    print(f"Message from server: {message.decode()}")

                except Exception as recv_error:
                    print(f"An error occurred while receiving data: {recv_error}")
                    break

    except ConnectionRefusedError:
        print("Connection failed. Make sure the server is running.")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()
